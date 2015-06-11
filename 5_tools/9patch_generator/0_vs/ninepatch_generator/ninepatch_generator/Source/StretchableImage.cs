using System;
using System.Linq;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;

namespace ninepatch_generator
{
    enum StretchableImageDirection
    {
        Vertical = 0,
        Horizontal = 1,
        Both = 2
    }
    /// <summary>
    /// Container for helper functions and easier accessability of areas associated to an image
    /// </summary>
    class AreaContainer
    {
        /// <summary>
        /// Dictionary of all Areas an object has, indexed by direction (horizontal/vertical)
        /// </summary>
        /// <seealso cref="Area">
        /// <seealso cref="StretchableImageDirection">
        public Dictionary<StretchableImageDirection, List<Area>> Areas;

        public AreaContainer()
        {
            Areas = new Dictionary<StretchableImageDirection, List<Area>>();
            Areas[StretchableImageDirection.Horizontal] = new List<Area>();
            Areas[StretchableImageDirection.Vertical]   = new List<Area>();
        }

        /// <summary>
        /// Allows for easy access of areas using simple filters
        /// </summary>
        /// <param name="direction">Which areas to get (horizontal/vertical/both)</param>
        /// <param name="isDynamic">Whether or not to get dynamic (true) or static (false) areas</param>
        /// <returns>List of all Areas matching the supplied criteria</returns>
        public List<Area> GetAreas(StretchableImageDirection direction, bool isDynamic)
        {
            List<Area> result = new List<Area>();

            IEnumerable<Area> eligibleAreas = (direction == StretchableImageDirection.Both) ? Areas.SelectMany(x => x.Value) : Areas[direction];
            foreach (Area area in eligibleAreas)
            {
                if (area.IsDynamic == isDynamic)
                {
                    result.Add(area);
                }
            }

            return result;
        }
    }

    class StretchableImage
    {
        /// <summary>
        /// Container of all areas read from the source image
        /// </summary>
        AreaContainer AreaContainer;
        /// <summary>
        /// Original non-cropped image supplied to generate an instance of this class
        /// </summary>
        Bitmap Source;
        /// <summary>
        /// [-2, -2]-center-aligned cropped version of ::Source
        /// </summary>
        Bitmap CutSource;

        #region String converter
        public override string ToString()
        {
            string result = "";

            result += string.Format(
                "Dimensions: {0}",
                CutSource.Size
            );
            result += "\n";

            result += string.Format(
                "Dynamic-area total: {0}",
                DynamicAreaTotal
            );
            result += "\n";

            result += string.Format(
                "Static-area total: {0}",
                StaticAreaTotal
            );
            result += "\n";
            result += "\n";


            result += string.Format(
                "Valid 9patch: {0}",
                IsValid().ToString()
            );
            result += "\n";
            result += "\n";


            result += "Horizontal Patches:\n";
            foreach (Area area in AreaContainer.Areas[StretchableImageDirection.Horizontal])
            {
                result += area;
                result += "\n";
            }
            result += "\n";

            result += "Vertical Patches:\n";
            foreach (Area area in AreaContainer.Areas[StretchableImageDirection.Vertical])
            {
                result += area;
                result += "\n";
            }
            result += "\n";

            return result;
        }
        static public implicit operator string(StretchableImage lhs)
        {
            return lhs.ToString();
        }
        #endregion

        /// <summary>
        /// Sum of all pixels noted as "static" (no black pixel) in ::Source
        /// </summary>
        private Size staticAreaTotal;
        /// <summary>
        /// Sum of all pixels noted as "dynamic" (no black pixel) in ::Source
        /// </summary>
        private Size dynamicAreaTotal;
        /// <summary>
        /// Public property to access the sum of all pixels noted as "static" (no black pixel) in ::Source
        /// </summary>
        public Size StaticAreaTotal
        {
            get
            {
                return staticAreaTotal;
            }
        }
        /// <summary>
        /// Public property to access the sum of all pixels noted as "dynamic" (no black pixel) in ::Source
        /// </summary>
        public Size DynamicAreaTotal
        {
            get
            {
                return dynamicAreaTotal;
            }
        }
        /// <summary>
        /// Denotes whether or not the image used to create an instance of this class was a valid 9-patch
        /// </summary>
        /// <returns>Is this instance using a valid image?</returns>
        public bool IsValid()
        {
            return (AreaContainer.Areas[StretchableImageDirection.Horizontal].Count + AreaContainer.Areas[StretchableImageDirection.Vertical].Count) > 2;
        }

        #region Constructor
        public StretchableImage(Image source)
        {
            AreaContainer = new AreaContainer();

            Source = new Bitmap(source);
            CutSource = new Bitmap(Source.Width - 2, Source.Height - 2);

            for (int y = 1; y < Source.Height - 1; y++)
            {
                for (int x = 1; x < Source.Width - 1; x++)
                {
                    CutSource.SetPixel(x-1, y-1, Source.GetPixel(x, y));
                }
            }

            GenerateAreas();
        }
        #endregion

        #region Internal functions
        /// <summary>
        /// Creates all patches using the image provided in the constructor and stores them accordingly
        /// </summary>
        private void CreatePatches()
        {
            // Dirty solution - optimize this to a more general function (don't seperate horizontal and vertical loop)
            #region Init
            bool IsDynamicArea = false;

            // We store pixels based on the CutSource (-1)
            int StartedAt = -1;
            int EndAt = -1;
            #endregion

            #region Horizontal
            IsDynamicArea = (Source.GetPixel(1, 0).ToArgb() == Color.Black.ToArgb());

            // We store pixels based on the CutSource (-1)
            StartedAt = 1;
            EndAt = -1;

            for (int x = 2; x < Source.Width - 1; x++)
            {
                if (IsDynamicArea != (Source.GetPixel(x, 0).ToArgb() == Color.Black.ToArgb()))
                {

                    EndAt = x - 1;

                    AreaContainer.Areas[StretchableImageDirection.Horizontal].Add(new Area(StartedAt, EndAt, true, IsDynamicArea));

                    StartedAt = x;

                    IsDynamicArea = (Source.GetPixel(x, 0).ToArgb() == Color.Black.ToArgb());
                }
            }
            AreaContainer.Areas[StretchableImageDirection.Horizontal].Add(new Area(StartedAt, Source.Width - 2, true, IsDynamicArea));
            #endregion

            #region Vertical
            IsDynamicArea = (Source.GetPixel(1, 0).ToArgb() == Color.Black.ToArgb());

            // We store pixels based on the CutSource (-1)
            StartedAt = 1;
            EndAt = -1;
            // Get vertical patch pixels
            for (int y = 2; y < Source.Height - 1; y++)
            {
                if (IsDynamicArea != (Source.GetPixel(0, y).ToArgb() == Color.Black.ToArgb()))
                {

                    EndAt = y - 1;

                    AreaContainer.Areas[StretchableImageDirection.Vertical].Add(new Area(StartedAt, EndAt, false, IsDynamicArea));

                    StartedAt = y;

                    IsDynamicArea = (Source.GetPixel(0, y).ToArgb() == Color.Black.ToArgb());
                }
            }
            AreaContainer.Areas[StretchableImageDirection.Vertical].Add(new Area(StartedAt, Source.Height - 2, false, IsDynamicArea));
            #endregion
        }
        /// <summary>
        /// Creates all patches and recalculates the total area size
        /// </summary>
        /// <seealso cref="dynamicAreaTotal">
        /// <seealso cref="staticAreaTotal">
        private void GenerateAreas()
        {
            CreatePatches();

            // Calculate area totals
            IEnumerable<Area> allAreas = AreaContainer.Areas.SelectMany(x => x.Value);
            foreach (Area area in allAreas)
            {
                if (area.IsDynamic)
                {
                    if (area.IsHorizontal)
                    {
                        dynamicAreaTotal.Width += area.Length;
                    }
                    else
                    {
                        dynamicAreaTotal.Height += area.Length;
                    }
                }
            }
            staticAreaTotal = CutSource.Size - dynamicAreaTotal;

            // Calculate weight based on totals
            foreach (Area area in allAreas)
            {
                area.Weight = (float)area.Length /
                              (area.IsDynamic ?
                                    (area.IsHorizontal ? (float)dynamicAreaTotal.Width : (float)dynamicAreaTotal.Height) :
                                    (area.IsHorizontal ? (float)staticAreaTotal.Width :  (float)staticAreaTotal.Height)
                              );
            }
        }
        #endregion

        #region Public functions
        /// <summary>
        /// Generates a new Bitmap using the Size supplied
        /// </summary>
        /// <param name="newSize">Size of the image to generate</param>
        /// <returns>A bitmap correctly stretched using the areas extracted from the image supplied in the constructor</returns>
        public Bitmap GenerateImage(Size newSize)
        {
            if (newSize.Width <= 0 || newSize.Height <= 0)
            {
                throw new Exceptions.StretchableImage_NullException(newSize.Width <= 0);
            }
            if (newSize.Width < staticAreaTotal.Width || newSize.Height < staticAreaTotal.Height)
            {
                throw new Exceptions.StretchableImage_TooSmallException(staticAreaTotal);
            }

            /// Determine canvas size
            // Needs to be at least CutSource-Size, since we first draw the x-stretched
            // CutSource and remove unwanted y-stretched areas afterwards 
            Bitmap temporaryCanvas = new Bitmap(newSize.Width > CutSource.Width ? newSize.Width : CutSource.Width, newSize.Height > CutSource.Height ? newSize.Height : CutSource.Height);
            Graphics temporaryCanvasGraphics = Graphics.FromImage(temporaryCanvas);

            Size DynamicSize = newSize - staticAreaTotal;       // Total size of possible dynamic area with new size

            // Determine Remainder
            Size DynamicSizeRemainder = DynamicSize;            // Remainder to split up between areas (2x0.5 areas splitting 3px would leave 1px here)
            foreach (Area area in AreaContainer.GetAreas(StretchableImageDirection.Both, true))
            {
                int PixelsOccupied = (int)Math.Floor( (float)(area.IsHorizontal ? DynamicSize.Width : DynamicSize.Height) * area.Weight );
                if (area.IsHorizontal)
                {
                    DynamicSizeRemainder.Width -= PixelsOccupied;
                }
                else
                {
                    DynamicSizeRemainder.Height -= PixelsOccupied;
                }
            }

            // Stretch image
            Size RemainingDynamicSize = DynamicSize;            // Total size of dynamic areas to fill
            Point CurrentPosition = new Point(0, 0);
            // X stretching
            foreach (Area area in AreaContainer.Areas[StretchableImageDirection.Horizontal])
            {
                if (area.IsDynamic)
                {
                    int PixelsOccupied = (int)Math.Floor((float)DynamicSize.Width * area.Weight);

                    // Slowly use up all Remainder pixels
                    if (DynamicSizeRemainder.Width > 0)
                    {
                        PixelsOccupied++;
                        DynamicSizeRemainder.Width--;
                    }

                    temporaryCanvasGraphics.DrawImage(CutSource, new Rectangle(CurrentPosition.X, 0, PixelsOccupied, CutSource.Height), new Rectangle(area.Start, 0, area.Length, CutSource.Height), GraphicsUnit.Pixel);

                    CurrentPosition.X += PixelsOccupied;
                    RemainingDynamicSize.Width -= PixelsOccupied;
                }
                else
                {
                    temporaryCanvasGraphics.DrawImage(CutSource, new Rectangle(CurrentPosition.X, 0, area.Length, CutSource.Height), new Rectangle(area.Start, 0, area.Length, CutSource.Height), GraphicsUnit.Pixel);

                    CurrentPosition.X += area.Length;
                }
            }

            // Y stretching
            Bitmap result = new Bitmap(newSize.Width, newSize.Height);
            Graphics resultGraphics = Graphics.FromImage(result);
            foreach (Area area in AreaContainer.Areas[StretchableImageDirection.Vertical])
            {
                if (area.IsDynamic)
                {
                    int PixelsOccupied = (int)Math.Floor((float)DynamicSize.Height * area.Weight);

                    // Slowly use up all Remainder pixels
                    if (DynamicSizeRemainder.Height > 0)
                    {
                        PixelsOccupied++;
                        DynamicSizeRemainder.Height--;
                    }

                    resultGraphics.DrawImage(temporaryCanvas, new Rectangle(0, CurrentPosition.Y, temporaryCanvas.Width, PixelsOccupied), new Rectangle(0, area.Start, temporaryCanvas.Width, area.Length), GraphicsUnit.Pixel);

                    CurrentPosition.Y += PixelsOccupied;
                    RemainingDynamicSize.Height -= PixelsOccupied;
                }
                else
                {
                    resultGraphics.DrawImage(temporaryCanvas, new Rectangle(0, CurrentPosition.Y, temporaryCanvas.Width, area.Length), new Rectangle(0, area.Start, temporaryCanvas.Width, area.Length), GraphicsUnit.Pixel);

                    CurrentPosition.Y += area.Length;
                }
            }

            return result;
        }
        #endregion
    }
}
