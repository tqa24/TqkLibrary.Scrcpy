using System.Drawing;
using TqkLibrary.Scrcpy;
using TqkLibrary.AdbDotNet;

Directory.GetFiles(Directory.GetCurrentDirectory(), "*.png")
    .ToList()
    .ForEach(File.Delete);

var devices = Adb.Devices(DeviceState.Device).ToList();

string deviceId = devices.First();
//string deviceId = "a29bc285";
//string deviceId = "5793d2f39905";


int i = 0;
ScrcpyConfig config = new ScrcpyConfig()
{
    HwType = FFmpegAVHWDeviceType.AV_HWDEVICE_TYPE_D3D11VA,
};
while (true)
{
    Console.WriteLine($"{DateTime.Now:mm:ss.fff} Start");
    using (Scrcpy scrcpy = new Scrcpy(deviceId))
    {
        Console.WriteLine($"{DateTime.Now:mm:ss.fff} Connect");
        scrcpy.Connect(config);
        Console.WriteLine($"{DateTime.Now:mm:ss.fff} Connected");
        await Task.Delay(3000);

        //while (true) await Task.Delay(3000);

        Console.WriteLine($"{DateTime.Now:mm:ss.fff} GetScreenShot");
        while (true)
        {
            using Bitmap bitmap = scrcpy.GetScreenShot();
            Console.WriteLine($"{DateTime.Now:mm:ss.fff} GetScreenShoted");
            bitmap.Save($"{i++:0000}.png");
            await Task.Delay(200);
            //break;
        }
        await Task.Delay(3000);

        while (true) await Task.Delay(3000);

        Console.WriteLine($"{DateTime.Now:mm:ss.fff} Stop");
        scrcpy.Stop();
        Console.WriteLine($"{DateTime.Now:mm:ss.fff} Stopped");
    }
    Console.WriteLine($"{DateTime.Now:mm:ss.fff} Disposesed");
    GC.Collect();
    GC.WaitForPendingFinalizers();
    Console.WriteLine($"{DateTime.Now:mm:ss.fff} GC cleared");
    //await Task.Delay(5000);
}