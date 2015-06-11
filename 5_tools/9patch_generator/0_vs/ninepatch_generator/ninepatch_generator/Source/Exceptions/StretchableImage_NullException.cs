using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ninepatch_generator.Exceptions
{
    class StretchableImage_NullException : Exception
    {
        public static string InfoText = "{0} size is null!";
        public bool IsHorizontal;
        public StretchableImage_NullException(bool isHorizontal)
        {
            IsHorizontal = isHorizontal;
        }

        /// <summary>
        /// Generates a user-readable debug-output of this object
        /// </summary>
        /// <returns>Human-readable info about this exception</returns>
        public override string ToString()
        {
            return string.Format(InfoText, IsHorizontal ? "Horizontal" : "Vertical");
        }
    }
}
