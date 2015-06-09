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
        /// Beginning pixel of this area in StretchableImage::CutSource-coordinates
        /// </summary>
        protected int start = 0;
        /// <summary>
        /// Ending pixel of this area in StretchableImage::CutSource-coordinates
        /// </summary>
        protected int end = 0;
        /// <summary>
        /// Proportion of the image (from 0.0f up to 1.0f) this area covers
        /// 
        /// -1.0f if still unset
        /// </summary>
        public float weight = -1.0f;

        /// <summary>
        /// Public property to access the beginning pixel of this area in StretchableImage::CutSource-coordinates
        /// </summary>
        public int Start
        {
            get
            {
                return start;
            }
        }
        /// <summary>
        /// Public property to access the ending pixel of this area in StretchableImage::CutSource-coordinates
        /// </summary>
        public int End
        {
            get
            {
                return end;
            }
        }
        /// <summary>
        /// Public property to access the proportion of the image (from 0.0 up to 1.0) this area covers
        /// </summary>
        public float Weight
        {
            get
            {
                return weight;
            }
            set
            {
                weight = value;
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
            return string.Format("{0} {1} [{2}, {3}, W: {4}]", IsHorizontal ? "H" : "V", IsDynamic ? "dyn" : "sta", Start, Length, Weight);
        }
        #endregion

        /// <summary>
        /// Creates a new area
        /// 
        /// Important: The constructor takes the absolute values from the StretchableImage::Source, but converts them to StretchableImage::CutSource-coordinates! ([-1, -1])
        /// </summary>
        /// <param name="start">Start pixel of this area in StretchableImage::Source-coordinates</param>
        /// <param name="end">Start pixel of this area in StretchableImage::Source-coordinates</param>
        /// <param name="isHorizontal">Whether or not this area is horizontally aligned</param>
        /// <param name="isDynamic">Whether or not this area is stretchable (black pixel)</param>
        public Area(int start, int end, bool isHorizontal, bool isDynamic)
        {
            this.start = start - 1;
            this.end = end - 1;
            IsHorizontal = isHorizontal;
            IsDynamic = isDynamic;
        }
    }
}
