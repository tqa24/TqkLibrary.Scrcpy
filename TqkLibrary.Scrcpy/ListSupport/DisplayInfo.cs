using System.Drawing;

namespace TqkLibrary.Scrcpy.ListSupport
{
    /// <summary>
    /// 
    /// </summary>
    public class DisplayInfo
    {
        /// <summary>
        /// 
        /// </summary>
        public string? Display { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public Size Size { get; set; }


        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            return $"{nameof(Display)}: {Display} - {nameof(Size)}: {Size.Width}x{Size.Height}";
        }
    }
}
