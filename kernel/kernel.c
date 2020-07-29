#include "hardware.h"
#include "segmentation.h"
#include "pm_timer.h"
#include "lapic_timer.h"
#include "util.h"
#include "interruption.h"

extern struct FrameBuffer *FB;
extern unsigned int fb_y;
extern unsigned int fb_x;
extern unsigned int lapic_timer_freq_khz;
extern struct InterrputDescripter IDT[256];

void start(void *SystemTable __attribute__ ((unused)), struct HardwareInfo *_hardware_info) {
  // From here - Put this part at the top of start() function
  // Do not use _hardware_info directry since this data is located in UEFI-app space
  hardware_info = *_hardware_info;
  FB = &(hardware_info.fb);
  fb_y=0;
  fb_x=0;

  init_segmentation();
  init_frame_buffer(FB);

  init_acpi_pm_timer(hardware_info.rsdp);
  unsigned int freq;
  freq  = measure_lapic_freq_khz();

  //8-A
  //unsigned long long handler;
  //asm volatile("lea print(%%rip), %[handler]":[handler]"=r"(handler));
  //lapic_periodic_exec(1000, (void*)handler); //print "ok" per one second

   // 8-B
  void *handler;
  asm volatile ("lea schedule(%%rip),%[handler]" : [handler]"=r"(handler));
  lapic_periodic_exec(1000, handler);
  init_tasks();

  // Do not delete it!
  while (1);
}
