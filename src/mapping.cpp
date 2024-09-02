
#include "main.h"
#include "mapping.h"
#include "hid_key.h"
#include "sed_key.h"


#include <list>
#include <set>


#define MAX_MAPPING_COUNT   (64)
#define MAX_KEY_STATE_COUNT (16)

typedef struct {
  uint32_t input;
  uint32_t output;
  uint16_t layers;
  uint16_t flags;
} mapping_t;


typedef struct {
  uint32_t key;
  uint32_t pressed_ts;
  uint32_t released_ts;  
} key_state_t;


typedef struct {
  uint16_t active_layers;
  uint32_t time;
  std::set<uint32_t> usages_in;
  std::set<uint32_t> usages_out;
} context_t;


static uint32_t  passthru_layers;             // passthru action
static uint16_t  mapping_count;
static mapping_t mapping[MAX_MAPPING_COUNT];  // mapping

static uint16_t active_layers;

static key_state_t key_state[MAX_KEY_STATE_COUNT];

class Action {
public:
  Action(uint32_t input, uint16_t layers, uint32_t timestamp) : _input(input), _layers(layers), _timestamp(timestamp) {};
  virtual ~Action() {}

  // Process this Action
  // Fills usages_out with produced usages at the processed time <now>
  // Returns true if the Action is completed, false if it needs further processing.
  virtual bool process(context_t& ctx) { return true; };

protected:
  uint32_t _input;
  uint16_t _layers;
  uint32_t _timestamp;
};


class KeyPressedAction : public Action {
public:
  KeyPressedAction(uint32_t input, uint32_t output, uint16_t layers, uint32_t timestamp) : 
    Action(input, layers, timestamp), _output(output) {};
  virtual bool process(context_t& ctx) override {
    if((ctx.active_layers & _layers)!=0)
    {
      ctx.usages_out.emplace(_output);
    }
    return true;
  }

protected:
  uint32_t _output;
};

class KeyTappedAction : public Action {
public:
  KeyTappedAction(uint32_t input, uint32_t output, uint16_t layers, uint32_t timestamp, uint32_t max_duration) : 
    Action(input, layers, timestamp), _output(output), _max_duration(max_duration) {};
  virtual bool process(context_t& ctx) override {
    if(ctx.usages_in.count(_input)==1) 
    {
      return false; 
    }
    if(ctx.time-_timestamp<_max_duration && (ctx.active_layers & _layers)!=0)
    {
      ctx.usages_out.emplace(_output);
    }
    return true;
  }

protected:
  uint32_t _output;
  uint32_t _max_duration;
};

class KeyHeldAction : public Action {
public:
  KeyHeldAction(uint32_t input, uint32_t output, uint32_t layers, uint32_t min_duration, uint32_t timestamp) : 
    Action(input, layers, timestamp), _output(output), _min_duration(min_duration){};
  virtual bool process(context_t& ctx) override {
    if(ctx.usages_in.count(_input)==0) 
    {
      return true;
    }
    if(ctx.time-_timestamp>=_min_duration && (ctx.active_layers & _layers)!=0)
    {
      ctx.usages_out.emplace(_output);
    }
    return true;
  }

protected:
  uint32_t _output;
  uint32_t _min_duration;
};





// static std::unordered_map<uint32_t, pair<uint32_t,list<mapping_t>>> pressed;   // 
// static std::unordered_map<uint32_t, pair<uint32_t,list<mapping_t>>> down;  // 
// static std::unordered_map<uint32_t, pair<uint32_t,list<mapping_t>>> released; // 


void init()
{
}


void process() {

  sed_report_t report;

  uint32_t now = millis();

  // Read pending message from queue
  report = {0} ;
  if(xQueueReceive(sed_input_msg_queue, &report, 0) == pdTRUE)
  {
    if(report.id==4 && report.type != HID_REPORT_INPUT && report.len>0 )
    {


      for(auto k: pressed)
      {
        if(k.first
      }

      for(int t=0; t<6; ++t)
      {
        uint32_t sed_key = report.payload.keys_in.keys[t];
        if(sed_key!=0) {
          sed_key |= MAPPING_USAGE_SED_KEY;



        }
      }      


    } 
    else 
    {
      if(report.len>0) 
      {
        xQueueSend(remapped_input_msg_queue, &report, 0);
      }
      report = {0};
    }
  }
  



}



