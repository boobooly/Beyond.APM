using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;

namespace BeyondAPM.Simple.Start
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                AppDomain currentAppDomain = AppDomain.CurrentDomain;
                currentAppDomain.UnhandledException += new UnhandledExceptionEventHandler(Handler);

                ProcessController procControl = new ProcessController(@"C:\liyan\IDE\NetProfiling\BAMP\SimpleDemo\BeyondAPM.Simple.Target\BeyondAPM.Simple.Target\bin\Debug\BeyondAPM.Simple.Target.exe");
                procControl.Start();
                char start = Console.ReadKey().KeyChar;
                if (start == (char)0)
                    procControl.Stop();
                
            }
            catch (Exception ex)
            {
                Console.Write(ex);
                Console.Write(ex.InnerException.StackTrace);
            }
            finally
            {
                Console.Read();
                //processProfiled.Close();
            }
        }
        static void Handler(object sender, UnhandledExceptionEventArgs args)
        {
            Exception e = (Exception)args.ExceptionObject;
            Console.WriteLine("the unhandled exception:" + e.Message);
            Console.WriteLine("the unhandled stacktrace:" + e.StackTrace);
        }
    }
}
