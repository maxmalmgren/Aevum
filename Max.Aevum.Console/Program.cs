using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace Max.Aevum.Console
{
    public class Program
    {
        static void Main(string[] args)
        {
            var timer = new HighResolutionTimer(1);

            int count = 0;
            DateTime start = DateTime.Now;

            var ticket = timer.Execute(() =>
                                           {
                                               count++;
                                               System.Console.WriteLine(count);
                                           }).At(Every.Milliseconds(10));


            Thread.Sleep(4000);
             
            timer.StopEvent(ticket);

            Thread.Sleep(6000);
        }
    }
}
