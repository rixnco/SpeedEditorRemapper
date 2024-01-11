
#include "Cli.h"
#include "debug.h"


Cli::Cli(uint8_t history_depth, uint16_t cmd_max_len, CliCmdHandler* handler, const char* prompt, const char* banner) :
  _io(nullptr),
  _handler(handler),
  _prompt(prompt),
  _banner(banner),
  _state(ERROR),
  _HISTORY_DEPTH(history_depth+1),
  _CMD_MAX_LEN(cmd_max_len)
{

  // Allocate commands buffer
  // We allocate memory to hold history_depth+2 commands
  // _cmd[         0      * (_CMD_MAX_LEN+1)] =  command (edition)
  // _cmd[         1      * (_CMD_MAX_LEN+1)] =  history command
  // _cmd[        ...                       ] =  ...
  // _cmd[         t                        ] =  ...                <- tail   : last history command
  // _cmd[        ...                       ] =  ...
  // _cmd[        ...                       ] =  ...
  // _cmd[        h-1                       ] =  ...                <- head-1 : 1st history command
  // _cmd[         h                        ] =  ...                <- head   : current prompt
  // _cmd[        ...                       ] =  ...
  // _cmd[(history_depth+2)*(_CMD_MAX_LEN+1)] =  history command
  _cmd = (char*) malloc(((history_depth+2)*(cmd_max_len+1)) * sizeof(char) );
  if(!_cmd) return;

  // Allocate history circular buffer
  _history = (char**) malloc(history_depth * sizeof(char*));
  if(!_history) {
    free(_cmd);
    _cmd = nullptr;
    return;
  }

  // Clear whole commands buffer (history+current+cmd)
  memclr(_cmd, (_HISTORY_DEPTH+1)*(_CMD_MAX_LEN+1));

  // Build history circular buffer
  for(int t=0; t<_HISTORY_DEPTH; ++t) {
    _history[t] = &_cmd[ (t+1) * (_CMD_MAX_LEN+1)];
  }

  // _history[(_history_head)*(_cmd_max_len+1)] = first history entry
  // _history[(_history_tail)*(_cmd_max_len+1)] = last history entry
  // _cmd = _history[_history_depth*(_cmd_max_len+1)] = current cmd line entry
  _history_head=_history_tail=_history_cnt=0;
  _history_idx = 0;

  // Current cmd line entry
  _cmd_idx=0;
  _cmd_len=0;
  
  // Current cursor position
  _cur_x=_cur_y=0; 

  // Clear escape buffer
  memclr(_esc_buffer, sizeof(_esc_buffer));
  _esc_idx = 0;

  _state=RESET;
}

Cli::~Cli()
{
  _io=nullptr;
  _handler=nullptr;
  _prompt= nullptr;
  _banner= nullptr;

  _cur_x=_cur_y=0; 

  if(_cmd) free(_cmd);
  _cmd = nullptr;
  _cmd_idx= _cmd_len=0;

  if(_history) free(_history);
  _history=nullptr;
  _history_head=_history_tail=_history_cnt=0;
  
  _state=RESET;

  _esc_idx = 0;

}

void Cli::setBanner(const char* banner)
{
  _banner = banner;
}

void Cli::setPrompt(const char* prompt)
{
  _prompt = prompt;
}

void Cli::setCmdHandler(CliCmdHandler *handler)
{
  _handler = handler;
}


bool Cli::begin(Stream* io)
{
  // Check if io is already defined
  if(_io!=nullptr) return false;
  
  // Set io stream
  _io = io;

  _state=RESET;
  return true;
}

void Cli::end()
{
  _io = nullptr;
  _state = RESET;
}

void Cli::reset()
{
  _state = RESET;
}

void Cli::update()
{
  static bool _ignore_next_lf=false;

  if(_io==nullptr) return;

  if(_state==RESET)
  {
    _clear();
    if(_banner!=nullptr) {
      _io->write(_banner, strlen(_banner));
    }
    _state = INIT;
  }

  if(_state==INIT)
  {
    memclr(_cmd, _CMD_MAX_LEN+1);
    _cmd_idx = 0;
    _cmd_len = 0;
    _screen_w= 0;
    _screen_h= 0;
    _cur_x = 0;
    _cur_y = 0;
    _io->printf("\033[G");        // Move cursor to left side.
    if(_prompt) _io->printf(_prompt);
    _io->printf("\033[s");        // save current position;
    _io->printf("\033[J");        // clear from cursor to end of screen
    _io->printf("\033[999;999H"); // move to 999,999
    _io->printf("\033[6n");       // request position (which will reflect screen size)
    _io->printf("\033[u");        // restore position
    _io->printf("\033[6n");       // request position (which will reflect the cursor position)
    _io->flush();
    _state = IDLE;
  } 

  while(_state>INIT && _io->available())
  {
    int c = _io->read();
    switch(_state)
    {
      case IDLE:
        switch(c) {
          case '\033':
            memclr(_esc_buffer, sizeof(_esc_buffer));
            _esc_idx= 0;
            _esc_buffer[_esc_idx++] = c;
            _state = ESC;
            break;
          case 0x7F:      // delete
            _delete();
            break;
          case 0x08:     // backspace
            _backspace();
            break;
          case '\n':
            if(_ignore_next_lf)
            {
              _ignore_next_lf=false;
              break;
            }
          case '\r':
          {
            _ignore_next_lf= c=='\r';
            _setCursor(_cmd_x_end, _cmd_y_end);
            _io->printf("\r\n"); _io->flush();

            _history_idx = 0;
            _state = INIT;

            DBG_PRINTF("Process cmd: _cmd_len=%d\n", _cmd_len);

            // Terminate command.
            _cmd[_cmd_len] = 0;
            
            if(_cmd_len==0) break;

            // right trim spaces;
            char* cmd = &_cmd[_cmd_len-1];
            while(_cmd_len && isSpace(*cmd)) { *cmd= 0; --cmd; --_cmd_len; }
            if(_cmd_len==0) break;

            // left trim spaces;
            cmd = _cmd;
            while(_cmd_len && isSpace(*cmd)) { *cmd=0; ++cmd; --_cmd_len; }
            if(_cmd_len==0) break;

            DBG_PRINTF("\ncmd: '%s'\n", cmd);

            // PROCESS COMMAND
            if(_handler) _handler->process(cmd, _io, this);

            // Update command history
            memclr(_history[_history_head], (_CMD_MAX_LEN+1));
            strncpy(_history[_history_head], cmd, _CMD_MAX_LEN);
            
            if(_history_cnt<_HISTORY_DEPTH-1) ++_history_cnt;
            _history_head = (_history_head+1) % _HISTORY_DEPTH;
            _history_tail = (_history_head-_history_cnt+_HISTORY_DEPTH) % _HISTORY_DEPTH;
            
            memclr(_history[_history_head], (_CMD_MAX_LEN+1));
            memclr(_cmd, (_CMD_MAX_LEN+1));
            break;
          }
          default:
            DBG_PRINTF("%02X ", c);
            if(_cmd_idx>=_CMD_MAX_LEN) break;
            // if(_cmd_len==0) {
            //   _cmd_x_start = _cmd_x = _cmd_x_end= _cur_x;
            //   _cmd_y_start = _cmd_y = _cmd_y_end= _cur_y;
            // }
            if( _cmd_idx<_cmd_len) {
              // We're inserting a character in the current command...
              // Shift the remaining chars before inserting.
              memmove(&_cmd[_cmd_idx+1], &_cmd[_cmd_idx], _cmd_len-_cmd_idx);
            }
            _cmd[_cmd_idx++]=c;
            _cmd_len++;
            _io->printf("%s", &_cmd[_cmd_idx-1]); _io->flush();

            _updateCursor();
        }
        break;
      case ESC:
        _esc_buffer[_esc_idx++] = c;
        if(c=='[') {
          // Prepare to parse parameters
          _esc_param= &_esc_buffer[_esc_idx];
          _esc_param_len = 0;
          _state = PARAM;
        } else {
          _esc_buffer[_esc_idx++] = c;
          _esc_buffer[_esc_idx]=0;
          _state = IDLE;
          _io->printf(_esc_buffer); _io->flush();
        }
        break;
      case PARAM:
        if(c>=0x30 && c<=0x3F)
        {
          if(_esc_idx<ESCAPE_BUFFER_MAX_LEN ) _esc_buffer[_esc_idx++] = c;
          break;
        }
        _esc_param_len = &_esc_buffer[_esc_idx] - _esc_param;
        // Proceed imediatly with INTERMEDIATE parsing;
        _esc_intermediate= &_esc_buffer[_esc_idx];
        _esc_intermediate_len=0;
        _state = INTERMEDIATE;
      case INTERMEDIATE:
        if(c>=0x20 && c<=0x2F)
        {
          if(_esc_idx<ESCAPE_BUFFER_MAX_LEN ) _esc_buffer[_esc_idx++] = c;
          break;
        }
        _esc_intermediate_len=&_esc_buffer[_esc_idx] - _esc_intermediate;
        // Proceed imediatly with FINAL parsing;
        _state= FINAL;
      case FINAL:
        if(_esc_idx<ESCAPE_BUFFER_MAX_LEN )  _esc_buffer[_esc_idx++]= c;
        _esc_buffer[_esc_idx]=0;
        switch(c) {
          case 'R': // Cursor position
          {
            uint16_t x=0;
            uint16_t y=0;
            if(_esc_param_len){
              y= strtoul(_esc_param, &_esc_param, 10);
              _esc_param++; // TODO CHECK that we have a ';'
              x= strtoul(_esc_param, &_esc_param, 10);
            }
            if(_screen_w==0 && _screen_h==0) { 
              _screen_w=x; _screen_h=y; 
            } else { 
              _cmd_x_start = _cmd_x = _cmd_x_end= _cur_x = x;
              _cmd_y_start = _cmd_y = _cmd_y_end= _cur_y = y;
            }

            DBG_PRINTF("pos: %hu,%hu\n", x,y);
            break;
          }
          case 'A': // Cursor UP
          {
            if(_history_idx==_history_cnt) break;

            if(_history_idx==0)
            {
              // Store current cmd buffer to history_head
              memclr(_history[_history_head], _CMD_MAX_LEN+1);
              strncpy(_history[_history_head], _cmd, _CMD_MAX_LEN);
            }
            ++_history_idx;

            // Copy history cmd to current cmd buffer
            memclr(_cmd, _CMD_MAX_LEN+1);
            uint16_t idx = (_history_head-_history_idx+_HISTORY_DEPTH) % _HISTORY_DEPTH;
            strncpy(_cmd, _history[idx], _CMD_MAX_LEN);
            
            _cmd_len = strlen(_cmd);
            _cmd_idx = _cmd_len;
            _updateCursor();
            _setCursor(_cmd_x_start, _cmd_y_start);
            _io->printf(_cmd);
            _io->printf("\033[J");
            _cmd_idx = _cmd_len;
            //_updateCursor();
            break;
          }
          case 'B': // Cursor DOWN
          {
            if(_history_idx==0) break;

            --_history_idx;

            // Copy history cmd to current cmd buffer
            memclr(_cmd, _CMD_MAX_LEN+1);
            uint16_t idx = (_history_head-_history_idx+_HISTORY_DEPTH) % _HISTORY_DEPTH;
            strncpy(_cmd, _history[idx], _CMD_MAX_LEN);
            
            _cmd_len = strlen(_cmd);
            _cmd_idx = _cmd_len;
            _updateCursor();
            _setCursor(_cmd_x_start, _cmd_y_start);
            _io->printf(_cmd);
            _io->printf("\033[J");
            _cmd_idx = _cmd_len;
            //_updateCursor();
            break;
          }
          case 'C': // Cursor RIGHT
            if(_cmd_idx<_cmd_len) {
              ++_cmd_idx;
              _updateCursor();
            } 
            break;
          case 'D': // Cursor LEFT
            if(_cmd_idx>0) {
              --_cmd_idx;
              _updateCursor();
            } 
            break;
          case '~': // VT Sequence
          {
            //
            uint16_t v=0; 
            if(_esc_param_len){
              v= strtoul(_esc_param, &_esc_param, 10);
            }
            switch(v) {
              case 1: // Home
              case 7: // Home
                _cmd_idx = 0;
                _updateCursor();
                break;
              case 3: // delete
                _delete();
                break;
              case 4: // End
              case 8: // End
                _cmd_idx = _cmd_len;
                _updateCursor();
                break;
              default:
                DBG_PRINTF("\n<ESC>%s ", &_esc_buffer[1]);
                DBG_FLUSH();
                break;
            }

            break;
          }
          case 'H': // Home
            _cmd_idx = 0;
            _updateCursor();
            break;
          case 'F': // End
            _cmd_idx = _cmd_len;
            _updateCursor();
            break;

          default:
            // Not implemented yet - or not supported
            _io->printf(_esc_buffer); _io->flush();
            DBG_PRINTF("\n<ESC>%s ", &_esc_buffer[1]);
            DBG_FLUSH();
            
            break;
        }
        _state = IDLE;
        break;
      default:
        break;
    }
  }
}

void Cli::_delete()
{
  if(_cmd_idx==_cmd_len) return;
  memmove(&_cmd[_cmd_idx], &_cmd[_cmd_idx+1], _cmd_len-_cmd_idx);
  --_cmd_len;
  _updateCursor();
  _io->printf("%s", &_cmd[_cmd_idx]); 
  _io->printf("\033[J");
  _io->flush();
  _updateCursor();
}

void Cli::_backspace()
{
  if(_cmd_idx==0) return;
  memmove(&_cmd[_cmd_idx-1], &_cmd[_cmd_idx], _cmd_len-_cmd_idx+1);
  --_cmd_idx;
  --_cmd_len;
  _updateCursor();
  _io->printf("%s", &_cmd[_cmd_idx]); 
  _io->printf("\033[J");
  _io->flush();
  _updateCursor();
}


void Cli::_updateCursor()
{
  _updateCursor(_cmd_idx);
}

void Cli::_updateCursor(uint16_t idx)
{
  uint16_t len = _cmd_len;
  if(idx>len) idx=len;
  if(idx<len) --len;

  _cmd_x_end = 1 + (((_cmd_x_start-1) + len) % _screen_w); 
  _cmd_y_end = _cmd_y_start + (((_cmd_x_start-1) + len) / _screen_w); 

  _cmd_x= 1 + (((_cmd_x_start-1) + idx) % _screen_w);
  _cmd_y= _cmd_y_start + (((_cmd_x_start-1) + idx) / _screen_w); 

  int32_t dy= _cmd_y_end - _screen_h ;

  // DBG_PRINTF("x: % 2u, % 2u, % 2u\n", _cmd_x_start, _cmd_x, _cmd_x_end);
  // DBG_PRINTF("y: % 2u, % 2u, % 2u\n", _cmd_y_start, _cmd_y, _cmd_y_end);

  if(dy>0) {
    _cmd_y_start  -= dy;
    _cmd_y_end    -= dy;
    _cmd_y        -= dy;
    if(_cmd_idx==_cmd_len) _io->printf("\033[%huS", dy);
  }
  _io->printf("\033[%hu;%huH", _cmd_y, _cmd_x);
}

#define MAX_SEQUENCE_LEN  12

void Cli::_clear()
{
  // explanation can be found here: http://braun-home.net/michael/info/misc/VT100_commands.htm
  _io->printf("\033[H");  // VT100 Home command
  _io->printf("\033[J" );  // VT100 screen erase command
}

void Cli::_setCursor(uint16_t x, uint16_t y)
{
  // explanation can be found here: http://braun-home.net/michael/info/misc/VT100_commands.htm
  _io->printf("\033[%hu;%huH",y,x);  
}


