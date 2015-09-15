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
RT_TASK demo_task4;
RT_TASK demo_task5;

void demo(void *arg)
{
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
}

int main(int argc, char* argv[])
{
  char  str[10] ;
  int   num1 = 1;
  int   num2 = 2;
  int   num3 = 3;
  int   num4 = 4;
  int   num5 = 5;

  // Perform auto-init of rt_print buffers if the task doesn't do so
  rt_print_auto_init(1);

  // Lock memory : avoid memory swapping for this program
  mlockall(MCL_CURRENT|MCL_FUTURE);

  rt_printf("start 5 tasks\n");

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
  rt_task_create(&demo_task2, str, 0, 51, 0);
  sprintf(str,"task3");
  rt_task_create(&demo_task3, str, 0, 52, 0);
  sprintf(str,"task4");
  rt_task_create(&demo_task4, str, 0, 53, 0);
  sprintf(str,"task5");
  rt_task_create(&demo_task5, str, 0, 54, 0);

  /*
   * Arguments: &task,
   *            task function,
   *            function argument
   */
  rt_task_start(&demo_task1, &demo, &num1);
  rt_task_start(&demo_task2, &demo, &num2);
  rt_task_start(&demo_task3, &demo, &num3);
  rt_task_start(&demo_task4, &demo, &num4);
  rt_task_start(&demo_task5, &demo, &num5);
}
