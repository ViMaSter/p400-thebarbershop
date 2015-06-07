using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ninepatch_generator.Source
{
    class WeightedArea : Area
    {
        /// <summary>
        /// Used when determining how many of the newly created pixels should be filled with this area
        /// </summary>
        public float Weight;

        public WeightedArea(Area area) : base(area.Start, area.End, area.IsHorizontal, area.IsDynamic)
        {
            Weight = 0.0f;
        }
    }
}
