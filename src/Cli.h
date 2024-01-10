#ifndef __CLI_H__
#define __CLI_H__
#include <Arduino.h>

#define DEFAULT_CLI_TIMEOUT_MS (1000) 
#define ESCAPE_BUFFER_MAX_LEN (28)

class Cli;

class CliCmdHandler
{
  public:
    virtual void process(const char* cmd, Print *p_out, Cli *p_in); 

};

class Cli 
{
  public:
    Cli();
    virtual ~Cli();

    void setCmdHandler(CliCmdHandler *callback);

    bool begin(Stream* io, uint8_t history_depth=5, uint16_t cmd_max_len=255);
    void end();
    void update();

    void clear();
    void home();
    bool getCursor(uint16_t &x, uint16_t &y, uint32_t timeout=DEFAULT_CLI_TIMEOUT_MS);
    void setCursor(uint16_t x, uint16_t y);

protected:
  typedef enum { 
    ERROR=-1,
    INIT,
    IDLE,
    FINAL,
    INTERMEDIATE,
    PARAM,
    CSI,
    ESC
  } parser_state_t;

  parser_state_t _state;
  char  _esc_buffer[ESCAPE_BUFFER_MAX_LEN+1];
  char  *_esc_param;
  uint16_t _esc_param_len;
  char  *_esc_intermediate;
  uint16_t _esc_intermediate_len;
  char  *_esc_final;
  
  uint16_t _esc_idx;

  Stream        *_io;
  CliCmdHandler *_handler;

  uint16_t    _screen_w,_screen_h;
  uint16_t    _cur_x,_cur_y;

  char        *_history;      // History command buffer
  uint8_t     _history_head;  // Head of the circular buffer command history
  uint8_t     _history_tail;  // Tail of the circular buffer command history
  uint8_t     _history_cnt;   // Number of element in the history buffer
  uint8_t     _history_idx;   // index of the history command we're currently editing (when navigating with the UP and DOWN arrow key)
  uint8_t     _HISTORY_DEPTH; // Maximum entries held in the history.
  
  char        *_cmd;          // Current command line entry (may be replaced by an history command)
  uint16_t    _cmd_idx;       // Position of the next inserted char in the command line.
  uint16_t    _cmd_len;       // Length of the current command line.
  
  uint16_t    _cmd_x_start, _cmd_x, _cmd_x_end;
  uint16_t    _cmd_y_start, _cmd_y, _cmd_y_end;

  uint16_t    _CMD_MAX_LEN;   // Maximun length of a command line. (buffer will hold _CMD_MAX_LEN+1 char to hold the '\0' terminating char)
};




#endif