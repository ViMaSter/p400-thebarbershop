using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ninepatch_generator.Exceptions
{
    class StretchableImage_TooSmallException : Exception
    {
        public static string InfoText = "Smallest allowed resolution: {0}x{1}px";
        public Size SmallestAllowedSize;
        public StretchableImage_TooSmallException(Size smallestAllowedSize)
        {
            SmallestAllowedSize = smallestAllowedSize;
        }

        /// <seealso cref="ToString()">
        static public implicit operator string(StretchableImage_TooSmallException lhs)
        {
            return lhs.ToString();
        }
        /// <summary>
        /// Generates a user-readable debug-output of this object
        /// </summary>
        /// <returns>Human-readable info about this exception</returns>
        public override string ToString()
        {
            return string.Format(InfoText, SmallestAllowedSize.Width, SmallestAllowedSize.Height);
        }
    }
}
