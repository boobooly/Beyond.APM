using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;
using System.Security.Permissions;
using System.IO;


namespace BeyondAPM.Simple.Agent
{
    public class AgentShim
    {
        //[HostProtection(SecurityAction.LinkDemand,UI=true)]
        //[System.Security.SecurityCritical(System.Security.SecurityCriticalScope.Everything)]
        public static void GetTracer(string msg,object target)
        {
            FileInfo fi = new FileInfo(@"c:\Agent.txt");
            FileStream fs = fi.Open(FileMode.OpenOrCreate, FileAccess.Write);
            try
            {
                string content = System.DateTime.Now.ToString("yyyyMMdd HH:mm:ss") + "\n";
                byte[] bContent = ASCIIEncoding.ASCII.GetBytes(content);
                fs.Write(ASCIIEncoding.ASCII.GetBytes(content), 0, ASCIIEncoding.ASCII.GetBytes(content).Length);

            }
            catch (IOException ex)
            {
            }
            finally
            {
                fs.Close();
            }
            Console.WriteLine(msg);
            Console.WriteLine(target.GetType());
            System.Console.WriteLine("This is AgentShim!");
        }
        //public static void()
        //[System.Security.SecurityCritical(System.Security.SecurityCriticalScope.Everything)]
        public static string GetTracer(string factory, string assembly, string classname, string methodname, uint traceParam, object target)
        {
            FileInfo fi = new FileInfo(@"c:\Agent.txt");
            FileStream fs = fi.Open(FileMode.OpenOrCreate, FileAccess.Write);
            try
            {
                string content = System.DateTime.Now.ToString("yyyyMMdd HH:mm:ss") + "\n";
                byte[] bContent = ASCIIEncoding.ASCII.GetBytes(content);
                fs.Write(bContent, 0, bContent.Length);

                StringBuilder sb = new StringBuilder();
                sb.Append("TracerFactory is :").Append(factory).AppendLine()
                    .Append("Assembly is :").Append(assembly).AppendLine()
                    .Append("Class is :").Append(classname).AppendLine()
                    .Append("Method is :").Append(methodname).AppendLine()
                    .Append("Target Type is ").Append(target.GetType()).AppendLine();
                byte[] bCon = ASCIIEncoding.ASCII.GetBytes(sb.ToString());
                fs.Write(bCon, 0, bCon.Length);
            }
            catch (IOException ex)
            {
            }
            finally
            {
                fs.Close();
            }
            //System.Console.WriteLine("This is AgentShim!");
            //StringBuilder sb =new StringBuilder();
            //sb.Append("TracerFactory is :").Append(factory).AppendLine()
            //        .Append("Assembly is :").Append(assembly).AppendLine()
            //        .Append("Class is :").Append(classname).AppendLine()
            //        .Append("Method is :").Append(methodname).AppendLine();
            //        //.Append("Target Type is ").Append(target.GetType()).AppendLine();
            //System.Console.Write(sb.ToString());
            return "GetTracer";
 
        }
        public static void FinishTracer(object obj)
        {
            FileInfo fi = new FileInfo(@"c:\Agent.txt");
            FileStream fs = fi.Open(FileMode.OpenOrCreate, FileAccess.Write);
            try
            {
                string content = System.DateTime.Now.ToString("yyyyMMdd HH:mm:ss") + "\n";
                byte[] bContent = ASCIIEncoding.ASCII.GetBytes(content);
                fs.Write(bContent, 0, bContent.Length);

                StringBuilder sb = new StringBuilder();
                sb.Append("FinishTracer is :").Append(obj).AppendLine();
                    //.Append("Assembly is :").Append(assembly).AppendLine()
                    //.Append("Class is :").Append(classname).AppendLine()
                    //.Append("Method is :").Append(methodname).AppendLine()
                    //.Append("Target Type is ").Append(target.GetType()).AppendLine();
                byte[] bCon = ASCIIEncoding.ASCII.GetBytes(sb.ToString());
                fs.Write(bCon, 0, bCon.Length);
            }
            catch (IOException ex)
            {
            }
            finally
            {
                fs.Close();
            }
            Console.WriteLine(obj.GetType());
        }
    }
}
