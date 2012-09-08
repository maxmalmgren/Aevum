using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using Machine.Specifications;

namespace Max.Aevum.Test
{
    public class When_executing_a_count_function
    {
        private static volatile int count;
        private static int epsilon = 25;

        Because of = () =>
                         {
                             using (var timer = new HighResolutionTimer(1))
                             {
                                 Action action = () =>
                                 {
                                     count++;
                                 };

                                 timer.Execute(action).At(Interval.Milliseconds(1));
                                 Thread.Sleep(10000);
                             }
                         };

        It should_execute_around_10000_times = () =>
                                                  {
                                                      count.ShouldBeGreaterThan(10000 - epsilon);
                                                      count.ShouldBeLessThan(10000 + epsilon);
                                                  };
    }
}
