using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;
using System.Diagnostics;

namespace BeyondAPM.Simple.Start
{
    public class ProcessController
    {
        private Process process;
        private string application;
        internal ProcessController(string application)
        {
            this.application = application;
        }
        public string Application
        {
            get { return this.application; }
            
        }
        public void Start()
        {
            ProcessStartInfo processStartInfo = new ProcessStartInfo(Application);

            processStartInfo.EnvironmentVariables["COR_ENABLE_PROFILING"] = "0x01";
            processStartInfo.EnvironmentVariables["COR_PROFILER"] = "{795638A6-2195-4499-B1CF-E00A595CA00F}";
            //.net framework 4.0 可以选择这个设置。
            processStartInfo.EnvironmentVariables["COR_PROFILER_PATH"] = @"C:\liyan\IDE\NetProfiling\BAMP\main\Debug\Beyond.APM.Profiler.dll";

            //processStartInfo.EnvironmentVariables["COR_ENABLE_PROFILING"] = "0x01";
            //processStartInfo.EnvironmentVariables["COR_PROFILER"] = "{8782F5A0-E8B0-49af-B9D2-D0BE025D5D3E}";
            //processStartInfo.EnvironmentVariables["COR_PROFILER_PATH"] = @"C:\liyan\IDE\NetProfiling\ILRewritting\Logger\Logger.Profiler\Debug\Logger.Profiler.dll";
            //processStartInfo.EnvironmentVariables["Logger_Profiler_Log"] = @"c:\Logger.Profiler.log";
            processStartInfo.UseShellExecute = false;
            RegisterDll.Register();
            process=Process.Start(processStartInfo);
            
        }
        public void Stop()
        {
            process.Kill();
        }

    }
}
