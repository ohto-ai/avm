#pragma once
#include <atlimage.h>

namespace thatboy
{
	namespace mfc
	{
		class CPng :
			public CImage
		{
		public:
			using CImage::CImage;
			virtual ~CPng() {};

			void AlphaPreMul()
			{
				for (int i = 0; i < CImage::GetWidth(); i++)
				{
					for (int j = 0; j < CImage::GetHeight(); j++)
					{
						LPBYTE pucColor = static_cast<LPBYTE>(CImage::GetPixelAddress(i, j));
						pucColor[0] = (pucColor[0] * pucColor[3] + 127) / 255;
						pucColor[1] = (pucColor[1] * pucColor[3] + 127) / 255;
						pucColor[2] = (pucColor[2] * pucColor[3] + 127) / 255;
					}
				}
			}

			HRESULT LoadPngFromResource(LPCTSTR pszResourceName, bool bIfAlphaPreMul = true, LPCTSTR lpImageResType = TEXT("PNG"))
			{
				HRESULT hres = NULL;
				HRSRC hrsrc = FindResource(NULL, pszResourceName, lpImageResType);
				if (hrsrc)
				{
					LPVOID lpsz = LockResource(LoadResource(NULL, hrsrc));
					ULARGE_INTEGER srcSize;
					srcSize.QuadPart = SizeofResource(NULL, hrsrc);

					IStream* pStream = NULL;
					CreateStreamOnHGlobal(NULL, TRUE, &pStream);
					if (pStream)
					{
						pStream->SetSize(srcSize);
						DWORD cbWritten = 0;
						pStream->Write(lpsz, srcSize.LowPart, &cbWritten);
						CImage::Destroy();
						hres = CImage::Load(pStream);
						pStream->Release();
					}
				}
				if (bIfAlphaPreMul)
					AlphaPreMul();
				return hres;
			}

			HRESULT LoadPngFromResource(UINT nReourceID, bool bIfAlphaPreMul = true, LPCTSTR lpImageResType = TEXT("PNG"))
			{
				return LoadPngFromResource(MAKEINTRESOURCE(nReourceID), bIfAlphaPreMul, lpImageResType);
			}

			HRESULT LoadPngFromFile(LPCTSTR pszFileName, bool bIfAlphaPreMul = true)
			{
				CImage::Destroy();
				HRESULT ret = CImage::Load(pszFileName);
				if (bIfAlphaPreMul)
					AlphaPreMul();
				return ret;
			}

			BOOL Create(int nWidth, int nHeight, int nBPP, DWORD dwFlags = 0)
			{
				CImage::Destroy();
				BOOL ret = CImage::Create(nWidth, nHeight, nBPP, dwFlags);
				CImage::SetHasAlphaChannel(true);
				return ret;
			}

			BOOL CreateEx(int nWidth, int nHeight, int nBPP, DWORD eCompression, _In_reads_opt_(3) const DWORD* pdwBitfields = NULL, DWORD dwFlags = 0)
			{
				CImage::Destroy();
				BOOL ret = CImage::CreateEx(nWidth, nHeight, nBPP, eCompression, pdwBitfields, dwFlags);
				CImage::SetHasAlphaChannel(true);
				return ret;
			}
		};

	}
}