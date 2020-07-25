#include "hardware.h"
#include "segmentation.h"
#include "pm_timer.h"
#include "lapic_timer.h"
#include "util.h"

extern struct FrameBuffer *FB;
extern unsigned int fb_y;
extern unsigned int fb_x;
extern unsigned int lapic_timer_freq_khz;

void start(void *SystemTable __attribute__ ((unused)), struct HardwareInfo *_hardware_info) {
  // From here - Put this part at the top of start() function
  // Do not use _hardware_info directry since this data is located in UEFI-app space
  hardware_info = *_hardware_info;
  FB = &(hardware_info.fb);
  fb_y=0;
  fb_x=0;
  init_segmentation();
  init_frame_buffer(FB);

  //7-A  
  puts("Hello,\rWorld!\n");
  puth(162539583,10);
  puts("\n");

  //7-B1
  unsigned int msec = 5000;
  init_acpi_pm_timer(hardware_info.rsdp);
  puts("wait for 5000msec \n");
  pm_timer_wait_milisec(msec);
  puts("waited \n");

  //7-B2
  lapic_timer_freq_khz = measure_lapic_freq_khz();
  puth(lapic_timer_freq_khz,8);

  // Do not delete it!
  while (1);

}