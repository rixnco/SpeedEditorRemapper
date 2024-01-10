
#include "Cli.h"


Cli::Cli() 
{
  _io=nullptr;
  _handler=nullptr;
  _cur_x=_cur_y=0; 
  _history=nullptr;
  _history_head=_history_tail=_history_cnt=0;
  _HISTORY_DEPTH=0;

  _cmd_idx=0;
  _cmd_len=0;
  _CMD_MAX_LEN=0;
}

Cli::~Cli()
{
  end();
}

void Cli::setCmdHandler(CliCmdHandler *callback)
{
  _handler = callback;
}

bool Cli::begin(Stream* io, uint8_t history_depth, uint16_t cmd_max_len)
{
  // Check if _history buffer is already allocated
  if(_history!=nullptr || io==nullptr) return false;
  
  // Allocate _history buffer
  _history = (char*) malloc((history_depth+1)*(cmd_max_len+1));
  if(!_history) return false;

  _HISTORY_DEPTH = history_depth;
  _CMD_MAX_LEN = cmd_max_len;
  memclr(_history, (_HISTORY_DEPTH+1)*(_CMD_MAX_LEN+1));

  // _history[(_history_head)*(_cmd_max_len+1)] = first history entry
  // _history[(_history_tail)*(_cmd_max_len+1)] = last history entry
  // _cmd = _history[_history_depth*(_cmd_max_len+1)] = current cmd line entry
  _history_head=_history_tail=_history_cnt=0;

  // Current cmd line entry
  _cmd= &_history[_HISTORY_DEPTH*(_CMD_MAX_LEN+1)];
  _cmd_idx=0;
  _cmd_len=0;
  
  // Current cursor position
  _cur_x=_cur_y=0; 

  // Clear escape buffer
  memclr(_esc_buffer, sizeof(_esc_buffer));
  _esc_idx = 0;

  // Set io stream
  _io = io;



  clear();

  // request cursor position
  if(getCursor(_cur_x, _cur_y))
  {
  }
  else
  {
  }

  _state=INIT;
  return true;
}

void Cli::end()
{
  if(_history) free(_history);
  _io=nullptr;
  _handler=nullptr;
  _cur_x=_cur_y=0; 
  _history=nullptr;
  _history_head=_history_tail=_history_cnt=0;

  _cmd = nullptr;
  _cmd_idx=0;
  _cmd_len=0;
  _state=INIT;

  _esc_idx = 0;
}


void Cli::update()
{
  

  if(_io==nullptr) return;

  if(_state==INIT)
  {
    memclr(_cmd, _CMD_MAX_LEN+1);
    _cmd_idx = 0;
    _cmd_len = 0;
    _screen_w= 0;
    _screen_h= 0;
    _cur_x = 0;
    _cur_y = 0;
    _io->printf("> "); _io->flush();
    _io->printf("\033[s");        // save current position;
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
          case '\r':
            //_io->printf("%c", c); _io->flush();
            break;
          case '\n':
            _io->printf("%c", c); _io->flush();
            // PROCESS COMMAND
            _state = INIT;
            break;
          default:
            if(_cmd_idx>=_CMD_MAX_LEN) break;
            if(_cmd_len==0) {
              _cmd_x_start = _cmd_x = _cmd_x_end= _cur_x;
              _cmd_y_start = _cmd_y = _cmd_y_end= _cur_y;
            }
            if( _cmd_idx<_cmd_len) {
              // We're inserting a character in the current command...
              // Shift the remaining chars before inserting.
              memmove(&_cmd[_cmd_idx+1], &_cmd[_cmd_idx], _cmd_len-_cmd_idx);
            }
            _cmd[_cmd_idx++]=c;
            _cmd_len++;
            _io->printf("%s", &_cmd[_cmd_idx-1]); _io->flush();
            if( _cmd_idx<_cmd_len) {
              _cmd_x= _cmd_x_start + (_cmd_idx % _screen_w);
              _cmd_y= _cmd_y_start + (_cmd_idx / _screen_w);
              // We have inserted a char.
              // We need to rewrite the end of the command line
              _io->printf("\033[s"); _io->flush(); // Save current cursor position
              _io->printf("%s", &_cmd[_cmd_idx]); ; _io->flush();  // write the end of the command line.
              _io->printf("\033[u"); _io->flush(); // Restore the cursor position
            }
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
        switch(c) {
          case 'R': // Cursor position
          {
            uint16_t x=0;
            uint16_t y=0;
            if(_esc_param_len){
              x= strtoul(_esc_param, &_esc_param, 10);
              _esc_param++; // TODO CHECK that we have a ';'
              y= strtoul(_esc_param, &_esc_param, 10);
            }
            if(_screen_w==0 && _screen_h==0) { _screen_w=x; _screen_h=y; }
            else { _cur_x=x; _cur_y=y; }

            Serial1.printf("pos: %hu,%hu\n", x,y);
            break;
          }
          case 'A': // Cursor UP
            c= -1;
            break;
          case 'B': // Cursor DOWN
            c= -1;
            break;
          case 'C': // Cursor RIGHT
            if(_cmd_idx<_cmd_len) {
              ++_cmd_idx;
              break;
            } 
            c= -1;
            break;
          case 'D': // Cursor LEFT
            if(_cmd_idx>0) {
              --_cmd_idx;
              break;
            } 
            c= -1;
            break;
          default:
            Serial1.printf("ESC: %c\n", c);
            // Not implemented yet - or not supported
            break;
        }
        if(c>=0)
        {
          if(_esc_idx<ESCAPE_BUFFER_MAX_LEN )  _esc_buffer[_esc_idx++]= c;
          _esc_buffer[_esc_idx]=0;
          _io->printf(_esc_buffer); _io->flush();
        }
        _state = IDLE;
        break;
      default:
        break;
    }
  }
}

#define MAX_SEQUENCE_LEN  12

void Cli::clear()
{
  // explanation can be found here: http://braun-home.net/michael/info/misc/VT100_commands.htm
  _io->printf("\033[H");  // VT100 Home command
  _io->printf("\033[J" );  // VT100 screen erase command
}

void Cli::setCursor(uint16_t x, uint16_t y)
{
  // explanation can be found here: http://braun-home.net/michael/info/misc/VT100_commands.htm
  _io->printf("\033[%hu;%huH");  
}


bool Cli::getCursor(uint16_t& x, uint16_t& y, uint32_t timeout)
{
  
  uint32_t start;
  uint16_t idx;
  
  char buffer[MAX_SEQUENCE_LEN+1];
  enum { ERROR=-1, DONE, Y, X, CSI, ESC } state;
 
  _io->printf("\033[6n"); _io->flush();
  idx = 0;
  start = millis();
  state = ESC;

  while( state>DONE && (millis()-start< timeout))
  {
    if(_io->available()) {
      int c= _io->read();
      switch(state)
      {
        case ESC:                   // ESC
          if(c=='\033') {
            state = CSI;
          } else {
            state=ERROR;
          }
          break;
        case CSI:                   // [
          if(c=='[') {
            state = X;
            idx=0;
          } else {
            state=ERROR;
          }
          break;
        case X:                   // <x>;
          if(c==';') {
            buffer[idx]=0;
            if(sscanf(buffer, "%hu",&x)!=1) {
              state=ERROR;
            } else {
              state=Y;
              idx=0;
            }
          } else if( c>='0' && c<='9') {
            buffer[idx++]= c;
          } else {
            state=ERROR;
          }
          break;
        case Y:                   // <y>R
          if(c=='R') {
            buffer[idx]=0;
            if(sscanf(buffer, "%hu",&y)!=1) {
              state=ERROR;
            } else {
              state=DONE;    
              idx=0;
            }
          } else if( c>='0' && c<='9') {
            buffer[idx++]= c;
          } else {
            state=ERROR;
          }
          break;
        default:
          state = ERROR;
      }
      if(idx>=MAX_SEQUENCE_LEN) state = ERROR;
    } else {
      yield();
    } 
  }
  return state == DONE;
}
