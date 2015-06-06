using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ninepatch_generator
{
    class Area
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
        public bool IsDynamic = false;

        #region String converter
        static public implicit operator string(Area lhs) {
            return lhs.ToString();
        }

        public override string ToString()
        {
            return string.Format("%s [%d, %d] %s", IsHorizontal ? "H" : "V", IsDynamic ? "dyn" : "sta", Start, Length);
        }
        #endregion

        public Area(int start, int length, bool isHorizontal, bool isDynamic)
        {
            this.start = start;
            this.length = length;
            IsHorizontal = isHorizontal;
            IsDynamic = isDynamic;
        }
    }
}
