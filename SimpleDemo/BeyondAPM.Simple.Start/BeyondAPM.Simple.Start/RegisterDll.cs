using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace BeyondAPM.Simple.Start
{
    public class RegisterDll
    {
        public RegisterDll()
        { }
        private static bool initialized = false;

        internal static bool Register()
        {
             int result;
            if(!initialized)
            {
                //需要先用系统管理员用户来 regsvr32 APM.Profiler.dll才会Dllregister成功。
                
                if ((result = DllRegisterServer()) == 0)
                    initialized = true;
                else
                {
                    //EventLog.WriteToEventLog("APM.Profiler", "APM.Agent.TestBootStrapper", "Couldn't register Profiler.dll");
                    Console.WriteLine("Couldn't register Profiler.dll");
                    throw new Exception("Couldn't register Profiler.dll");
                }
             }
            return initialized;
        }
        //[DllImport(@"C:\liyan\IDE\NetProfiling\ILRewritting\Logger\Debug\APM.Profiler.dll")]
        [DllImport(@"C:\liyan\IDE\NetProfiling\BAMP\main\Debug\Beyond.APM.Profiler.dll")]
        private static extern int DllRegisterServer();

    
    }
}
