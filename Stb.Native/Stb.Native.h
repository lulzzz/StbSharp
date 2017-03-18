// Stb.Native.h

#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Runtime::InteropServices;

#include <stdio.h>
#include <vector>

#define STBI_NO_STDIO
#define STB_IMAGE_IMPLEMENTATION
#include "../StbSharp.Generator/StbSource/stb_image.h"

#define STBI_WRITE_NO_STDIO
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../StbSharp.Generator/StbSource/stb_image_write.h"

namespace StbNative {
	void write_func(void *context, void *data, int size);

	public ref class Native
	{
	public:
		// TODO: Add your methods for this class here.
		static array<unsigned char> ^ load_from_memory(array<unsigned char> ^bytes, [Out] int %x, [Out] int %y, [Out] int %comp, int req_comp)
		{
			pin_ptr<unsigned char> p = &bytes[0];

			int xx, yy, ccomp;
			const unsigned char *ptr = (const unsigned char *)p;
			const unsigned char *res = stbi_load_from_memory(ptr, bytes->Length, &xx, &yy, &ccomp, req_comp);

			x = xx;
			y = yy;
			comp = ccomp;

			array<unsigned char> ^result = gcnew array<unsigned char>(x * y * req_comp);
			for (int i = 0; i < result->Length; ++i)
			{
				result[i] = res[i];
			}

			return result;
		}

		static Stream^ stream;

		// TODO: Add your methods for this class here.
		static void save_to_memory(array<unsigned char> ^bytes, int x, int y, int comp, int type, Stream ^output)
		{
			stream = output;

			pin_ptr<unsigned char> p = &bytes[0];
			unsigned char *ptr = (unsigned char *)p;

			switch (type)
			{
				case 0:
					stbi_write_bmp_to_func(write_func, nullptr, x, y, comp, ptr);
					break;
				case 1:
					stbi_write_tga_to_func(write_func, nullptr, x, y, comp, ptr);
					break;
				case 2:
				{
					std::vector<float> ff;
					ff.resize(bytes->Length);
					for (int i = 0; i < bytes->Length; ++i)
					{
						ff[i] = (float)(bytes[i] / 255.0f);
					}

					stbi_write_hdr_to_func(write_func, nullptr, x, y, comp, &ff[0]);
					break;
				}
				case 3:
					stbi_write_png_to_func(write_func, nullptr, x, y, comp, ptr, x * comp);
					break;
			}

			stream = nullptr;
		}
	};

	void write_func(void *context, void *data, int size)
	{
		unsigned char *bptr = (unsigned char *)data;
		for (int i = 0; i < size; ++i)
		{
			Native::stream->WriteByte(*bptr);
			++bptr;
		}
	}
}
