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

            //var ticket = timer.Execute(() =>
            //                               {
            //                                   count++;
            //                                   System.Console.WriteLine(count);
            //                               }).At(Every.Milliseconds(10));

            var ticket = timer.Execute(() =>
                                           {
                                               count++;
                                               System.Console.WriteLine("Exiting sleep after: " + (DateTime.Now - start).TotalMilliseconds);
                                           }).RepeatedlyWithDelay(1);

            Thread.Sleep(400);
        }
    }
}
