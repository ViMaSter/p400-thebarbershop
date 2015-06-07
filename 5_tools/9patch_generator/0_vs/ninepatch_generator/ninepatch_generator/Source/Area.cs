using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ninepatch_generator
{
    class Area
    {
        /// <summary>
        /// Beginning pixel of this area
        /// </summary>
        protected int start = 0;
        /// <summary>
        /// Ending pixel of this area
        /// </summary>
        protected int end = 0;

        /// <summary>
        /// Public property to access the beginning pixel of this area
        /// </summary>
        public int Start
        {
            get
            {
                return start;
            }
        }
        /// <summary>
        /// Public property to access the ending pixel of this area
        /// </summary>
        public int End
        {
            get
            {
                return end;
            }
        }
        /// <summary>
        /// Public property to access the length ((::end - ::start) + 1) of this area
        /// </summary>
        public int Length {
            get {
                return (end - start)+1;
            }
        }

        /// <summary>
        /// Denotes whether or not this area is horizontally aligned
        /// </summary>
        public bool IsHorizontal = false;
        /// <summary>
        /// Denotes whether or not this area is stretchable (black pixel)
        /// </summary>
        public bool IsDynamic = false;

        #region String converter
        /// <seealso cref="ToString()">
        static public implicit operator string(Area lhs) {
            return lhs.ToString();
        }
        /// <summary>
        /// Generates a user-readable debug-output of this object
        /// </summary>
        /// <returns>String containing all variables of this object</returns>
        public override string ToString()
        {
            return string.Format("{0} {1} [{2}, {3}]", IsHorizontal ? "H" : "V", IsDynamic ? "dyn" : "sta", Start, Length);
        }
        #endregion

        public Area(int start, int end, bool isHorizontal, bool isDynamic)
        {
            this.start = start;
            this.end = end;
            IsHorizontal = isHorizontal;
            IsDynamic = isDynamic;
        }
    }
}
