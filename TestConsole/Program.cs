// See https://aka.ms/new-console-template for more information
using TqkLibrary.Scrcpy;

Console.WriteLine("Hello, World!");
using (Scrcpy scrcpy = new Scrcpy("5793d2f39905"))
{
    scrcpy.Connect();
}
Console.WriteLine("Success");