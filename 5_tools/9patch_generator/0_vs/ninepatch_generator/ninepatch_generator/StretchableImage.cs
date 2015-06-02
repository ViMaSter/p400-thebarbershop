using System;
using System.Collections.Generic;
using System.Drawing.Imaging;

namespace ninepatch_generator
{
    class StretchableImage
    {
        Dictionary<bool /*IsHorizontal*/, List<StretchableArea>> StretchableAreas;
        System.Drawing.Bitmap Source;
        System.Drawing.Bitmap CutSource;
        
        public StretchableImage(System.Drawing.Image source)
        {
            StretchableAreas = new Dictionary<bool, List<StretchableArea>>();
            StretchableAreas[true] = new List<StretchableArea>();
            StretchableAreas[false] = new List<StretchableArea>();

            Source = new System.Drawing.Bitmap(source);
            CutSource = new System.Drawing.Bitmap(Source.Width - 2, Source.Height - 2);

            for (int y = 1; y < Source.Height - 1; y++)
            {
                for (int x = 1; x < Source.Width - 1; x++)
                {
                    CutSource.SetPixel(x-1, y-1, Source.GetPixel(x, y));
                }
            }

            GenerateAreasFromImage();
        }

        public void GenerateAreasFromImage()
        {
            int StartedAt = -1;

            // get horizontals
            for (int x = 0; x < Source.Width - 1; x++)
            {
                if (Source.GetPixel(x, 0).ToArgb() == System.Drawing.Color.Black.ToArgb())
                {
                    if (StartedAt == -1)
                    {
                        StartedAt = x;
                    }
                }
                else
                {
                    if (StartedAt != -1)
                    {
                        StretchableAreas[false].Add(new StretchableArea(StartedAt, x - StartedAt, false));
                        StartedAt = -1;
                    }
                }
            }

            if (StartedAt != -1)
            {
                StretchableAreas[false].Add(new StretchableArea(StartedAt, (Source.Width - 1) - StartedAt, false));
                StartedAt = -1;
            }

            // get verticals
            for (int y = 0; y < Source.Height - 1; y++)
            {
                if (Source.GetPixel(0, y).ToArgb() == System.Drawing.Color.Black.ToArgb())
                {
                    if (StartedAt == -1)
                    {
                        StartedAt = y;
                    }
                }
                else
                {
                    if (StartedAt != -1)
                    {
                        StretchableAreas[true].Add(new StretchableArea(StartedAt, y - StartedAt, false));
                        StartedAt = -1;
                    }
                }
            }

            if (StartedAt != -1)
            {
                StretchableAreas[true].Add(new StretchableArea(StartedAt, (Source.Height - 1) - StartedAt, false));
                StartedAt = -1;
            }
        }
    }
}
