using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ninepatch_generator
{
    class StretchableArea
    {
        private int start = 0;
        private int length = 0;

        public int Start
        {
            get {
                return start;
            }
        } 
        public int Length {
            get {
                return length;
            }
        }

        public bool IsHorizontal = false;

        #region String converter
        static public implicit operator string(StretchableArea lhs) {
            return lhs.ToString();
        }

        public override string ToString()
        {
            return string.Format("[%d, %d]", Start, Length);
        }
        #endregion

        public StretchableArea(int start, int length, bool isHorizontal)
        {
            this.start = start;
            this.length = length;
            IsHorizontal = isHorizontal;
        }
    }
}
