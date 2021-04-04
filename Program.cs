using System;
using OpenHardwareMonitor.Hardware;
using System.Threading;
using System.IO.Ports;

namespace Get_CPU_Temp5
{
    class Program
    {
        public class Update : IVisitor
        {
            public void VisitComputer(IComputer MyPC)
            {
                MyPC.Traverse(this);
            }
            public void VisitHardware(IHardware MyHardware)
            {
                MyHardware.Update();
                foreach (IHardware HardwardElement in MyHardware.SubHardware) HardwardElement.Accept(this);
            }
            public void VisitSensor(ISensor sensor) { }
            public void VisitParameter(IParameter parameter) { }
        }

        /*
        Sensors for Hardware[0]     CPU
            0 to 5 Individual Core Usage %
            6 average CPU Usage 
            7 to 12 Indivudual Cores Temperature
            13 average CPU Temperature
            14 to 19 Indivudual Core Frequency
            20 CPU Package W
            22 CPU DRam
            23 BUS Speed
         
        Sensors for Hardware[1]     GPU
            0 Temperature
            1 Core Speed
            2 Memory
            3 Shader
            4 GPU ID
            5 GPU Usage
            6 Frame Buffer
            7 Video Engine ????
            8 BUS Interface
            9 Fan
            10 Memory Total
            11 Memory Used
            12 Memory Free
            13 Memory ???
        Sensors for Hardware[2]     MEMORY
            1   Used RAM
            2   Available RAM
         */
        static void GetSystemInfo(Update update1, Computer MyComputer1, SerialPort port1)
        {
            string CPUTemp = null;
            string CPUuse = null;
            string GPUTemp = null;
            string GPUuse = null;
            string RAMFree = null;
            int CPUuseInt = 0;
            MyComputer1.Accept(update1);
            /* *** CPU PACKAGE TEMPERATURE *** */
           // Console.WriteLine(MyComputer1.Hardware[0].Sensors[13].Name + ":" + MyComputer1.Hardware[0].Sensors[13].Value.ToString().PadRight(4) + "\r");

            CPUTemp = MyComputer1.Hardware[0].Sensors[13].Value.ToString().PadRight(4);

            /* *** AVERAGE CPU USAGE *** */
            CPUuseInt = (int)MyComputer1.Hardware[0].Sensors[6].Value; //This conversion is to get rid of decimals.
            CPUuse = CPUuseInt.ToString().PadRight(4);

            /* *** GPU TEMPERATURE *** */
            GPUTemp = MyComputer1.Hardware[1].Sensors[0].Value.ToString().PadRight(4);

            /* *** AVERAGE CPU USAGE *** */
            GPUuse = MyComputer1.Hardware[1].Sensors[5].Value.ToString().PadRight(4);

            /* *** FREE MEMORY AVAILABLE *** */
            RAMFree = MyComputer1.Hardware[2].Sensors[2].Value.ToString().PadRight(4);

            port1.Write(CPUTemp + CPUuse + GPUTemp + GPUuse + RAMFree);
            Console.WriteLine(CPUTemp + CPUuse + GPUTemp + GPUuse + RAMFree + "\r");
        }

        static void Main(string[] args)
        {
            SerialPort port;
            port = new SerialPort("COM6", 9600, Parity.None, 8, StopBits.One);
            port.Open();
            Update update = new Update();
            Computer MyComputer = new Computer();
            MyComputer.Open();
            MyComputer.CPUEnabled = true;
            MyComputer.GPUEnabled = true;
            MyComputer.RAMEnabled = true;
            MyComputer.Accept(update);

            while (true)
            {
                GetSystemInfo(update, MyComputer, port);
                Thread.Sleep(1000);
            }
        }
    }
}