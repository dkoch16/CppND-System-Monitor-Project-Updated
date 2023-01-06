#ifndef PROCESSOR_H
#define PROCESSOR_H


class Processor {
 public:
  Processor();
  float Utilization();

 private:
 
 /**
  * @brief saved idle jiffies 
  **/
 long _prevIdleJiffies;
  
  /**
  * @brief saved total jiffies 
  **/
 long _prevTotalJiffies;
};

#endif