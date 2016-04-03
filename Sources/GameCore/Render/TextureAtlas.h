#ifndef	__GAMECORE_TEXTUREATLAS_H__
#define __GAMECORE_TEXTUREATLAS_H__

namespace SDK
{
	namespace Render
	{

		class TextureAtlas
		{
		private:
			std::vector<Vector3i> m_nodes;
			size_t m_width;
			size_t m_height;

			size_t m_deepth;

			size_t m_in_use;

			std::unique_ptr<ubyte[]> mp_data;

		private:
			int AtlasFit(const size_t i_index, const size_t i_width, const size_t i_height);
			void Merge();

		public:
			TextureAtlas(size_t i_width, size_t i_height, size_t i_depth);

			IRect GetNewRegion(const size_t i_width, const size_t i_height);
			void SetRegion(const IRect& i_region, const ubyte* ip_data, const size_t i_stride);

			void Clear();

			size_t Width() const { return m_width; }
			size_t Height() const { return m_height; }
			size_t Deepth() const { return m_deepth; }
			size_t InUse() const { return m_in_use; }
			const ubyte* GetDatPtr() const { return mp_data.get(); }
		};

	} // Render
} // SDK

#endif
