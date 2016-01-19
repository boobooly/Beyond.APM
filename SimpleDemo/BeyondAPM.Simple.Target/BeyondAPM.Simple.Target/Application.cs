using System;
using System.Collections.Generic;
using System.Text;

namespace BeyondAPM.Simple.Target
{
    public class Application
    {
        internal interface IExecuteStep
        {
            void Execute();
        }
        [System.Security.Permissions.SecurityPermission(System.Security.Permissions.SecurityAction.Assert,SkipVerification=true)]
        public void Show(string msg)
        {
            Console.WriteLine("Application.Show is waiting for input:");
            Console.ReadLine();
            System.Diagnostics.Debugger.Break();
            string name = "system.web";
            Console.WriteLine(name);
            Application.IExecuteStep obj = new ExecutionStep();
            obj.Execute();
            Console.WriteLine(msg);
        }
        internal class ExecutionStep : Application.IExecuteStep
        {
            void Application.IExecuteStep.Execute()
            {
                Console.WriteLine("ExecutionStep.Application.IExecuteStep.Execute.");
            }
        }
    }

    //internal class ExecutionStep : Application.IExecuteStep
    //{
    //     void Application.IExecuteStep.Execute()
    //    {
    //        Console.WriteLine("ExecutionStep.Application.IExecuteStep.Execute.");
    //    }
    //}
}
