#pragma once

#include <d3d9.h>

namespace slv
{
	namespace modules
	{
		namespace render
		{
			struct DxBufParams
			{
				enum Type { Static = 0, Dynamic = D3DUSAGE_DYNAMIC };
				enum Clipping { Clip = 0, DontClip = D3DUSAGE_DONOTCLIP };
				enum OptimizedPrimitives
				{
					None = 0,
					NPatches = D3DUSAGE_NPATCHES,
					Points = D3DUSAGE_POINTS,
					RTPatches = D3DUSAGE_RTPATCHES
				};
				enum VertexProcessing
				{
					Hardware = 0,
					Software = D3DUSAGE_SOFTWAREPROCESSING
				};
				enum Other
				{
					NonSecure = D3DUSAGE_NONSECURE,
					TextApi = D3DUSAGE_TEXTAPI,
					WriteOnly = D3DUSAGE_WRITEONLY
				};

				Type bufferType;
				Clipping clipping;
				OptimizedPrimitives optimizedPrimitives;
				Other otherFlags;
				VertexProcessing vertexProcessing;

				operator DWORD() const
				{
					return static_cast<DWORD>(bufferType | clipping
						| optimizedPrimitives | vertexProcessing | otherFlags);
				}
			};
			enum LockParams
			{
				None = 0,
				Discard = D3DLOCK_DISCARD,
				NoDirtyUpdate = D3DLOCK_NO_DIRTY_UPDATE,
				NoSysLock = D3DLOCK_NOSYSLOCK,
				// ReadOnly = D3DLOCK_READONLY,
				NoOverwrite = D3DLOCK_NOOVERWRITE
			};
		}
	}
}