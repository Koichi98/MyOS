int strcmp(char* str1, char* str2);

void init_acpi_pm_timer(struct RSDP *rsdp);

void pm_timer_wait_milisec(unsigned int msec);