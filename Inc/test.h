#ifndef TEST_H
#define TEST_H

#ifdef __cplusplus
extern "C" {
  
  
  class adc_ctrl
  {
  public:
    adc_ctrl();
    static uint adc_pd_average; // PD average
    void test();
  private:
    
    
  };
  
  extern class adc_ctrl adc1;

}
#endif
  void adc_ttest();
#endif