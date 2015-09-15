#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>

#include <native/task.h>
#include <native/timer.h>

#include  <rtdk.h>
RT_TASK demo_task1;
RT_TASK demo_task2;
RT_TASK demo_task3;

void demo(void *arg)
{
  RT_TASK *curtask;
  RT_TASK_INFO curtaskinfo;
  int num = * (int *) arg;
  unsigned long long period = 1000000000LLU * (unsigned long long) num;

  // periodic mode
  rt_task_set_periodic(NULL, TM_NOW, period);

  // inquire current task
  curtask=rt_task_self();
  rt_task_inquire(curtask,&curtaskinfo);
  
  // print task name
  rt_printf("Task name : %s \n", curtaskinfo.name);
 
  // sleep one second
  rt_task_sleep(1000000000LLU);

  while (1) {
    rt_printf("%d \n", num);
    rt_task_wait_period(NULL);
  }
  return;
}
/*
  RT_TASK *curtask;
  RT_TASK_INFO curtaskinfo;
  int num = *(int *) arg;

  // inquire current task
  curtask=rt_task_self();
  rt_task_inquire(curtask,&curtaskinfo);

  // print task name
  rt_printf("Task name : %s \n", curtaskinfo.name);
  // print argument
  rt_printf("Task argument : %d \n", num);
}*/

int main(int argc, char* argv[])
{
  char  str[10] ;
  int   num1 = 1;
  int   num2 = 2;
  int   num3 = 3;

  // Perform auto-init of rt_print buffers if the task doesn't do so
  rt_print_auto_init(1);

  // Lock memory : avoid memory swapping for this program
  mlockall(MCL_CURRENT|MCL_FUTURE);

  rt_printf("start 3 tasks periodically\n");

  /*
   * Arguments: &task,
   *            name,
   *            stack size (0=default),
   *            priority,
   *            mode (FPU, start suspended, ...)
   */
  sprintf(str,"task1");
  rt_task_create(&demo_task1, str, 0, 50, 0);
  sprintf(str,"task2");
  rt_task_create(&demo_task2, str, 0, 50, 0);
  sprintf(str,"task3");
  rt_task_create(&demo_task3, str, 0, 50, 0);

  /*
   * Arguments: &task,
   *            task function,
   *            function argument
   */
  rt_task_start(&demo_task1, &demo, &num1);
  rt_task_start(&demo_task2, &demo, &num2);
  rt_task_start(&demo_task3, &demo, &num3);

  rt_printf("end program by CTRL-C\n");
  pause();
}
