namespace TqkLibrary.Scrcpy.ListSupport
{
    /// <summary>
    /// 
    /// </summary>
    public class CodecInfo
    {
        /// <summary>
        /// 
        /// </summary>
        public string? Codec { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string? Encoder { get; set; }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            return $"{nameof(Codec)}: {Codec} - {nameof(Encoder)}: {Encoder}";
        }
    }
}
