using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;

namespace BeyondAPM.Simple.Target
{
    public class Program
    {
        public static void Main(string[] args)
        {
            System.Diagnostics.Debugger.Break();
            AppDomain currentAppDomain = AppDomain.CurrentDomain;
            currentAppDomain.UnhandledException += new UnhandledExceptionEventHandler(Handler);
            Application obj = new Application();
            obj.Show("this is Application");

            try
            {
                Program.DemoMethod();
            }
            catch (Exception e)
            {
                Console.Write(e.StackTrace);
                Console.WriteLine();
                Console.Write(e.InnerException.StackTrace);
            }
        }
        public static void DemoMethod()
        {
            System.Console.WriteLine("This is DemoMethod");
            System.Console.WriteLine("another line");
        }
        static void Handler(object sender, UnhandledExceptionEventArgs args)
        {
            Exception e = (Exception)args.ExceptionObject;
            Console.WriteLine("the unhandled exception:" + e.Message);
            Console.WriteLine("the unhandled inner exception:" + e.InnerException);
        }
    }
}
