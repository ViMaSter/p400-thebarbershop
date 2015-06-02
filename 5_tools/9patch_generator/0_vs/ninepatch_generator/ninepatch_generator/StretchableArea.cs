using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ninepatch_generator
{
    class StretchableArea
    {
        int Start = 0;
        int Length = 0;
        bool IsHorizontal = false;

        public StretchableArea(int start, int length, bool isHorizontal)
        {
            Start = start;
            Length = length;
            IsHorizontal = isHorizontal;
        }
    }
}
