#pragma once

#include <bit>
#include <cmath>
#include "bit_utils.hpp"
#include "types.hpp"

static constexpr u8 OP_FLUSH = 4;
static constexpr u8 OP_START_TILE_BINNING = 6;
static constexpr u8 OP_END_OF_RENDERING = 13;
static constexpr u8 OP_RETURN_FROM_SUB_LIST = 18;
static constexpr u8 OP_FLUSH_VCD_CACHE = 19;
static constexpr u8 OP_START_ADDRESS_OF_GENERIC_TILE_LIST = 20;
static constexpr u8 OP_BRANCH_TO_IMPLICIT_TILE_LIST = 21;
static constexpr u8 OP_SUPERTILE_COORDINATES = 23;
static constexpr u8 OP_CLEAR_TILE_BUFFERS = 25;
static constexpr u8 OP_END_OF_LOADS = 26;
static constexpr u8 OP_END_OF_TILE_MARKER = 27;
static constexpr u8 OP_STORE_TILE_BUFFER_GENERAL = 29;
static constexpr u8 OP_LOAD_TILE_BUFFER_GENERAL = 30;
static constexpr u8 OP_VERTEX_ARRAY_PRIMS = 36;
static constexpr u8 OP_SET_INSTANCEID = 54;
static constexpr u8 OP_PRIM_LIST_FORMAT = 56;
static constexpr u8 OP_GL_SHADER_STATE = 64;
static constexpr u8 OP_VCM_CACHE_SIZE = 71;
static constexpr u8 OP_TRANSFORM_FEEDBACK_SPECS = 74;
static constexpr u8 OP_BLEND_CONSTANT_COLOR = 86;
static constexpr u8 OP_COLOR_WRITE_MASKS = 87;
static constexpr u8 OP_ZERO_ALL_CENTROID_FLAGS = 88;
static constexpr u8 OP_SAMPLE_STATE = 91;
static constexpr u8 OP_OCCLUSION_QUERY_COUNTER = 92;
static constexpr u8 OP_CFG_BITS = 96;
static constexpr u8 OP_ZERO_ALL_FLAT_SHADE_FLAGS = 97;
static constexpr u8 OP_ZERO_ALL_NON_PERSPECTIVE_FLAGS = 99;
static constexpr u8 OP_POINT_SIZE = 104;
static constexpr u8 OP_LINE_WIDTH = 105;
static constexpr u8 OP_CLIP_WINDOW = 107;
static constexpr u8 OP_VIEWPORT_OFFSET = 108;
static constexpr u8 OP_CLIPPER_Z_MIN_MAX_CLIPPING_PLANES = 109;
static constexpr u8 OP_CLIPPER_XY_SCALING = 110;
static constexpr u8 OP_CLIPPER_Z_SCALE_AND_OFFSET = 111;
static constexpr u8 OP_NUMBER_OF_LAYERS = 119;
static constexpr u8 OP_TILE_BINNING_MODE_CFG = 120;
static constexpr u8 OP_TILE_RENDERING_MODE_CFG_COMMON = 121;
static constexpr u8 OP_TILE_RENDERING_MODE_CFG_COLOR = 121;
static constexpr u8 OP_TILE_RENDERING_MODE_CFG_ZS_CLEAR_VALUES = 121;
static constexpr u8 OP_TILE_RENDERING_MODE_CFG_CLEAR_COLORS_PART1 = 121;
static constexpr u8 OP_MULTICORE_RENDERING_SUPERTILE_CFG = 122;
static constexpr u8 OP_MULTICORE_RENDERING_TILE_LIST_SET_BASE = 123;
static constexpr u8 OP_TILE_COORDINATES = 124;
static constexpr u8 OP_TILE_COORDINATES_IMPLICIT = 125;
static constexpr u8 OP_TILE_LIST_INITIAL_BLOCK_SIZE = 126;

// these crimes are committed to make sure that unused bits are zero initialized
#define UNUSED_BITS3(t, n, x) t __##x : n = 0
#define UNUSED_BITS2(t, n, x) UNUSED_BITS3(t, n, x)
#define UNUSED_BITS(t, n) UNUSED_BITS2(t, n, __LINE__)

struct CLPacket
{
    CLPacket(u8 op) : opcode(op) {}
    u8 opcode;
} PACKED;

struct NumberOfLayers : CLPacket
{
    NumberOfLayers(u8 n) : CLPacket(OP_NUMBER_OF_LAYERS), layers(n - 1) {}
    u8 layers;
} PACKED;
static_assert(sizeof(NumberOfLayers) == 2);

struct TileBinningModeCfg : CLPacket
{
    TileBinningModeCfg(u8 initAllocBlockSize, u8 allocBlockSize, u8 targetCount,
                       u8 maxBpp, bool msaa, bool doubleBuffer, u16 w, u16 h) :
        CLPacket(OP_TILE_BINNING_MODE_CFG),
        tile_allocation_initial_block_size(initAllocBlockSize),
        tile_allocation_block_size(allocBlockSize),
        number_of_render_targets(targetCount - 1),
        maximum_bpp_of_all_render_targets(maxBpp),
        multisample_mode_4x(msaa),
        double_buffer_in_non_ms_mode(doubleBuffer),
        width_in_pixels(w - 1),
        height_in_pixels(h - 1)
    {
    }

    UNUSED_BITS(u8, 2);
    // 0=64b; 1 = 128b; 2=256b
    u8 tile_allocation_initial_block_size : 2;
    u8 tile_allocation_block_size : 2;
    UNUSED_BITS(u8, 2);

    u8 number_of_render_targets : 4;
    // (0=32bpp, 2=128bpp)
    u8 maximum_bpp_of_all_render_targets : 2;
    u8 multisample_mode_4x : 1;
    u8 double_buffer_in_non_ms_mode : 1;

    UNUSED_BITS(u8, 8);
    UNUSED_BITS(u8, 8);
    u16 width_in_pixels;
    u16 height_in_pixels;
} PACKED;
static_assert(sizeof(TileBinningModeCfg) == 9);

struct TileRenderingModeCfgCommon : CLPacket
{
    TileRenderingModeCfgCommon(u8 targetCount, u16 w, u16 h, u8 maxBpp,
                               bool msaa, bool doubleBuffer, u8 zTestDir,
                               bool earlyZDisable, u8 internalDepthType,
                               bool earlyStencilClear) :
        CLPacket(OP_TILE_RENDERING_MODE_CFG_COMMON),
        sub_id(0),
        number_of_render_targets(targetCount - 1),
        image_width_pixels(w),
        image_height_pixels(h),
        maximum_bpp_of_all_render_targets(maxBpp),
        multisample_mode_4x(msaa),
        double_buffer_in_non_ms_mode(doubleBuffer),
        early_z_test_and_update_direction(zTestDir),
        early_z_disable(earlyZDisable),
        internal_depth_type(internalDepthType),
        early_depth_stencil_clear(earlyStencilClear)
    {
    }

    u8 sub_id : 4;
    u8 number_of_render_targets : 4;
    u16 image_width_pixels;
    u16 image_height_pixels;
    // 0=32bpp; 1=64bpp; 2=128bpp
    u16 maximum_bpp_of_all_render_targets : 2;
    u16 multisample_mode_4x : 1;
    u16 double_buffer_in_non_ms_mode : 1;
    UNUSED_BITS(u16, 1);
    u16 early_z_test_and_update_direction : 1;
    u16 early_z_disable : 1;
    // 0=32F; 1=24; 2=16
    u16 internal_depth_type : 4;
    u16 early_depth_stencil_clear : 1;
    UNUSED_BITS(u16, 4);

    UNUSED_BITS(u8, 8);
} PACKED;
static_assert(sizeof(TileRenderingModeCfgCommon) == 9);

struct TileRenderingModeCfgColor : CLPacket
{
    TileRenderingModeCfgColor(u8 bpp0, u8 type0, u8 clamp0, u8 bpp1, u8 type1,
                              u8 clamp1, u8 bpp2, u8 type2, u8 clamp2, u8 bpp3,
                              u8 type3, u8 clamp3) :
        CLPacket(OP_TILE_RENDERING_MODE_CFG_COLOR),
        sub_id(1),
        render_target_0_internal_bpp(bpp0),
        render_target_0_internal_type(type0),
        render_target_0_clamp(clamp0),
        render_target_1_internal_bpp(bpp1),
        render_target_1_internal_type(type1),
        render_target_1_clamp(clamp1),
        render_target_2_internal_bpp(bpp2),
        render_target_2_internal_type(type2),
        render_target_2_clamp(clamp2),
        render_target_3_internal_bpp(bpp3),
        render_target_3_internal_type(type3),
        render_target_3_clamp(clamp3)
    {
    }

    u64 sub_id : 4;
    u64 render_target_0_internal_bpp : 2;
    u64 render_target_0_internal_type : 4;
    u64 render_target_0_clamp : 2;
    u64 render_target_1_internal_bpp : 2;
    u64 render_target_1_internal_type : 4;
    u64 render_target_1_clamp : 2;
    u64 render_target_2_internal_bpp : 2;
    u64 render_target_2_internal_type : 4;
    u64 render_target_2_clamp : 2;
    u64 render_target_3_internal_bpp : 2;
    u64 render_target_3_internal_type : 4;
    u64 render_target_3_clamp : 2;
    UNUSED_BITS(u64, 28);

} PACKED;
static_assert(sizeof(TileRenderingModeCfgColor) == 9);

struct TileRenderingModeCfgZSClearValues : CLPacket
{
    TileRenderingModeCfgZSClearValues(u8 stencilClearVlue, float zClearValue) :
        CLPacket(OP_TILE_RENDERING_MODE_CFG_ZS_CLEAR_VALUES),
        sub_id(2),
        stencil_clear_value(stencilClearVlue),
        z_clear_value(zClearValue)
    {
    }

    u8 sub_id;
    u8 stencil_clear_value;
    float z_clear_value;
    UNUSED_BITS(u8, 8);
    UNUSED_BITS(u8, 8);
} PACKED;
static_assert(sizeof(TileRenderingModeCfgZSClearValues) == 9);

struct TileRenderingModeCfgClearColorsPart1 : CLPacket
{
    TileRenderingModeCfgClearColorsPart1(u8 targetIdx, u32 colorLow,
                                         u32 colorNext) :
        CLPacket(OP_TILE_RENDERING_MODE_CFG_CLEAR_COLORS_PART1),
        sub_id(3),
        render_target_number(targetIdx),
        clear_color_low_32_bits(colorLow),
        clear_color_next_24_bits(colorNext)
    {
    }

    u64 sub_id : 4;
    u64 render_target_number : 4;
    u64 clear_color_low_32_bits : 32;
    u64 clear_color_next_24_bits : 24;
} PACKED;
static_assert(sizeof(TileRenderingModeCfgClearColorsPart1) == 9);

struct TileListInitialBlockSize : CLPacket
{
    TileListInitialBlockSize(u8 blockSize, bool autoChained) :
        CLPacket(OP_TILE_LIST_INITIAL_BLOCK_SIZE),
        size_of_first_block_in_chained_tile_lists(blockSize),
        use_auto_chained_tile_lists(autoChained)
    {
    }

    // 0=64b; 1 = 128b; 2=256b
    u8 size_of_first_block_in_chained_tile_lists : 2;
    u8 use_auto_chained_tile_lists : 1;
    UNUSED_BITS(u8, 5);
};
static_assert(sizeof(TileListInitialBlockSize) == 2);

struct OcclusionQueryCounter : CLPacket
{
    OcclusionQueryCounter(u32 addr) :
        CLPacket(OP_OCCLUSION_QUERY_COUNTER),
        address(addr)
    {
    }

    u32 address;
} PACKED;
static_assert(sizeof(OcclusionQueryCounter) == 5);

struct MulticoreRenderingTileListSetBase : CLPacket
{
    MulticoreRenderingTileListSetBase(u32 addr) :
        CLPacket(OP_MULTICORE_RENDERING_TILE_LIST_SET_BASE),
        address(addr >> 6)
    {
        assert((addr & 0x3F) == 0);
    }

    u32 tile_list_set_number : 4 = 0;
    UNUSED_BITS(u32, 2);
    u32 address : 26;
} PACKED;
static_assert(sizeof(MulticoreRenderingTileListSetBase) == 5);

struct MulticoreRenderingSupertileCfg : CLPacket
{
    MulticoreRenderingSupertileCfg(u32 superTileW, u32 superTileH,
                                   u32 frameWSupertile, u32 frameHSupertile,
                                   u32 frameWTile, u32 frameHTile,
                                   u32 binTileListCount, bool rasterOrder,
                                   bool multicoreEnabled) :
        CLPacket(OP_MULTICORE_RENDERING_SUPERTILE_CFG),
        supertile_width_in_tiles(superTileW - 1),
        supertile_height_in_tiles(superTileH - 1),
        total_frame_width_in_supertiles(frameWSupertile),
        total_frame_height_in_supertiles(frameHSupertile),
        total_frame_width_in_tiles(frameWTile),
        total_frame_height_in_tiles(frameHTile),
        multicore_enable(multicoreEnabled),
        supertile_raster_order(rasterOrder),
        number_of_bin_tile_lists(binTileListCount - 1)
    {
    }

    u8 supertile_width_in_tiles;
    u8 supertile_height_in_tiles;
    u8 total_frame_width_in_supertiles;
    u8 total_frame_height_in_supertiles;
    u32 total_frame_width_in_tiles : 12;
    u32 total_frame_height_in_tiles : 12;
    u8 multicore_enable : 1;
    UNUSED_BITS(u8, 3);
    u8 supertile_raster_order : 1;
    u8 number_of_bin_tile_lists : 3;
} PACKED;
static_assert(sizeof(MulticoreRenderingSupertileCfg) == 9);

struct TileCoordinates : CLPacket
{
    TileCoordinates(u32 col, u32 row) :
        CLPacket(OP_TILE_COORDINATES),
        tile_column_number(col),
        tile_row_number(row)
    {
    }
    u32 tile_column_number : 12;
    u32 tile_row_number : 12;
} PACKED;
static_assert(sizeof(TileCoordinates) == 4);

static constexpr u32 BUFFER_RENDER_TARGET_0 = 0;
static constexpr u32 BUFFER_RENDER_TARGET_1 = 1;
static constexpr u32 BUFFER_RENDER_TARGET_2 = 2;
static constexpr u32 BUFFER_RENDER_TARGET_3 = 3;
static constexpr u32 BUFFER_NONE = 8;
static constexpr u32 BUFFER_Z = 9;
static constexpr u32 BUFFER_STENCIL = 10;
static constexpr u32 BUFFER_ZSTENCIL = 11;

struct StoreTileBufferGeneral : CLPacket
{
    StoreTileBufferGeneral() :
        CLPacket(OP_STORE_TILE_BUFFER_GENERAL),
        buffer_to_store(BUFFER_NONE)
    {
    }
    StoreTileBufferGeneral(u32 bufferType, u32 memFormat, bool flipY,
                           u32 ditherMode, u32 decimateMode, u32 outputFormat,
                           bool clear, bool channelReverse, bool rbSwap,
                           u32 heightUbStride, u32 height, u32 addr) :
        CLPacket(OP_STORE_TILE_BUFFER_GENERAL),
        buffer_to_store(bufferType),
        memory_format(memFormat),
        flip_y(flipY),
        dither_mode(ditherMode),
        decimate_mode(decimateMode),
        output_image_format(outputFormat),
        clear_buffer_being_stored(clear),
        channel_reverse(channelReverse),
        r_b_swap(rbSwap),
        height_in_ub_or_stride(heightUbStride),
        height(height),
        address(addr)
    {
    }

    u8 buffer_to_store : 4 = 0;
    u8 memory_format : 3 = 0;
    u8 flip_y : 1 = 0;

    u16 dither_mode : 2 = 0;
    u16 decimate_mode : 2 = 0;
    u16 output_image_format : 6 = 0;
    u16 clear_buffer_being_stored : 1 = 0;
    u16 channel_reverse : 1 = 0;
    u16 r_b_swap : 1 = 0;
    UNUSED_BITS(u16, 3);

    UNUSED_BITS(u32, 4);
    u32 height_in_ub_or_stride : 20 = 0;
    u16 height = 0;
    u32 address = 0;
} PACKED;
static_assert(sizeof(StoreTileBufferGeneral) == 13);

struct ClearTileBuffers : CLPacket
{
    ClearTileBuffers(bool clearAllRenderTargets, bool clearZStencil) :
        CLPacket(OP_CLEAR_TILE_BUFFERS),
        clear_all_render_targets(clearAllRenderTargets),
        clear_z_stencil_buffer(clearZStencil)
    {
    }

    u8 clear_all_render_targets : 1;
    u8 clear_z_stencil_buffer : 1;
    UNUSED_BITS(u8, 6);
} PACKED;
static_assert(sizeof(ClearTileBuffers) == 2);

struct StartAddressOfGenericTileList : CLPacket
{
    StartAddressOfGenericTileList(u32 start, u32 end) :
        CLPacket(OP_START_ADDRESS_OF_GENERIC_TILE_LIST),
        start(start),
        end(end)
    {
    }

    u32 start;
    u32 end;
} PACKED;
static_assert(sizeof(StartAddressOfGenericTileList) == 9);

struct LoadTileBufferGeneral : CLPacket
{
    LoadTileBufferGeneral(u32 bufferType, u32 memFormat, bool flipY,
                          u32 decimateMode, u32 imgFormat, bool forceAlpha1,
                          bool channelReverse, bool rbSwap, u32 heightUbStride,
                          u32 height, u32 addr) :
        CLPacket(OP_LOAD_TILE_BUFFER_GENERAL),
        buffer_to_load(bufferType),
        memory_format(memFormat),
        flip_y(flipY),
        decimate_mode(decimateMode),
        input_image_format(imgFormat),
        force_alpha_1(forceAlpha1),
        channel_reverse(channelReverse),
        r_b_swap(rbSwap),
        height_in_ub_or_stride(heightUbStride),
        height(height),
        address(addr)
    {
    }

    u8 buffer_to_load : 4;
    u8 memory_format : 3;
    u8 flip_y : 1;

    UNUSED_BITS(u16, 2);
    u16 decimate_mode : 2;
    u16 input_image_format : 6;
    u16 force_alpha_1 : 1;
    u16 channel_reverse : 1;
    u16 r_b_swap : 1;

    UNUSED_BITS(u32, 4);
    u32 height_in_ub_or_stride : 20;

    u16 height;
    u32 address;

} PACKED;
static_assert(sizeof(LoadTileBufferGeneral) == 13);

static constexpr u32 LIST_POINTS = 0;
static constexpr u32 LIST_LINES = 1;
static constexpr u32 LIST_TRIANGLES = 2;

struct PrimListFormat : CLPacket
{
    PrimListFormat(u32 primType, bool stripOrFan) :
        CLPacket(OP_PRIM_LIST_FORMAT),
        primitive_type(primType),
        tri_strip_or_fan(stripOrFan)
    {
    }

    u8 primitive_type : 6;
    UNUSED_BITS(u8, 1);
    u8 tri_strip_or_fan : 1;
} PACKED;
static_assert(sizeof(PrimListFormat) == 2);

struct SetInstanceId : CLPacket
{
    SetInstanceId(u32 id) : CLPacket(OP_SET_INSTANCEID), instance_id(id) {}

    u32 instance_id;
} PACKED;
static_assert(sizeof(SetInstanceId) == 5);

struct BranchToImplicitTileList : CLPacket
{
    BranchToImplicitTileList(u8 tileListSetNumber) :
        CLPacket(OP_BRANCH_TO_IMPLICIT_TILE_LIST),
        tile_list_set_number(tileListSetNumber)
    {
    }

    u8 tile_list_set_number;
} PACKED;
static_assert(sizeof(BranchToImplicitTileList) == 2);

struct SupertileCoorinates : CLPacket
{
    SupertileCoorinates(u8 x, u8 y) :
        CLPacket(OP_SUPERTILE_COORDINATES),
        column_number_in_supertiles(x),
        row_number_in_supertiles(y)
    {
    }

    u8 column_number_in_supertiles;
    u8 row_number_in_supertiles;

} PACKED;
static_assert(sizeof(SupertileCoorinates) == 3);

struct ClipWindow : CLPacket
{
    ClipWindow(u16 left, u16 bottom, u16 w, u16 h) :
        CLPacket(OP_CLIP_WINDOW),
        clip_window_left_pixel_coordinate(left),
        clip_window_bottom_pixel_coordinate(bottom),
        clip_window_width_in_pixels(w),
        clip_window_height_in_pixels(h)
    {
    }

    u16 clip_window_left_pixel_coordinate;
    u16 clip_window_bottom_pixel_coordinate;
    u16 clip_window_width_in_pixels;
    u16 clip_window_height_in_pixels;
} PACKED;
static_assert(sizeof(ClipWindow) == 9);

struct CfgBits : CLPacket
{
    CfgBits(bool ffPrim, bool rfPrim, bool clockWise, bool depthOff,
            u32 lineRaster, u32 overSampleMode, bool d3dWireframeMode,
            u32 depthTestFunc, bool zUpdate, bool earlyZ, bool earlyZUpdate,
            bool stencil, bool blend, bool d3dPointFill, bool d3dprovokingVtx) :
        CLPacket(OP_CFG_BITS),
        enable_forward_facing_primitive(ffPrim),
        enable_reverse_facing_primitive(rfPrim),
        clockwise_primitives(clockWise),
        enable_depth_offset(depthOff),
        line_rasterization(lineRaster),
        rasterizer_oversample_mode(overSampleMode),
        direct3d_wireframe_triangles_mode(d3dWireframeMode),
        depth_test_function(depthTestFunc),
        z_updates_enable(zUpdate),
        early_z_enable(earlyZ),
        early_z_updates_enable(earlyZUpdate),
        stencil_enable(stencil),
        blend_enable(blend),
        direct3d_point_fill_mode(d3dPointFill),
        direct3d_provoking_vertex(d3dprovokingVtx)
    {
    }

    u8 enable_forward_facing_primitive : 1;
    u8 enable_reverse_facing_primitive : 1;
    u8 clockwise_primitives : 1;
    u8 enable_depth_offset : 1;
    u8 line_rasterization : 2;
    u8 rasterizer_oversample_mode : 2;

    UNUSED_BITS(u8, 3);
    u8 direct3d_wireframe_triangles_mode : 1;
    u8 depth_test_function : 3;
    u8 z_updates_enable : 1;

    u8 early_z_enable : 1;
    u8 early_z_updates_enable : 1;
    u8 stencil_enable : 1;
    u8 blend_enable : 1;
    u8 direct3d_point_fill_mode : 1;
    u8 direct3d_provoking_vertex : 1;
    UNUSED_BITS(u8, 2);
} PACKED;
static_assert(sizeof(CfgBits) == 4);

struct PointSize : CLPacket
{
    PointSize(float size) : CLPacket(OP_POINT_SIZE), point_size(size) {}
    f32 point_size;
} PACKED;
static_assert(sizeof(PointSize) == 5);

struct LineWidth : CLPacket
{
    LineWidth(float size) : CLPacket(OP_LINE_WIDTH), line_width(size) {}
    f32 line_width;
} PACKED;
static_assert(sizeof(LineWidth) == 5);

struct ClipperXYScaling : CLPacket
{
    ClipperXYScaling(f32 w, f32 h) :
        CLPacket(OP_CLIPPER_XY_SCALING),
        viewport_half_width_in_1_256th_of_pixel(w),
        viewport_half_height_in_1_256th_of_pixel(h)
    {
    }

    f32 viewport_half_width_in_1_256th_of_pixel;
    f32 viewport_half_height_in_1_256th_of_pixel;
} PACKED;
static_assert(sizeof(ClipperXYScaling) == 9);

struct ClipperZScaleAndOffset : CLPacket
{
    ClipperZScaleAndOffset(f32 scale, f32 offset) :
        CLPacket(OP_CLIPPER_Z_SCALE_AND_OFFSET),
        viewport_z_scale_zc_to_zs(scale),
        viewport_z_offset_zc_to_zs(offset)
    {
    }

    f32 viewport_z_scale_zc_to_zs;
    f32 viewport_z_offset_zc_to_zs;
} PACKED;
static_assert(sizeof(ClipperZScaleAndOffset) == 9);

struct CLipperZMinMaxClippingPlanes : CLPacket
{
    CLipperZMinMaxClippingPlanes(f32 min, f32 max) :
        CLPacket(OP_CLIPPER_Z_MIN_MAX_CLIPPING_PLANES),
        minimum_zw(min),
        maximum_zw(max)
    {
    }

    f32 minimum_zw;
    f32 maximum_zw;
} PACKED;
static_assert(sizeof(CLipperZMinMaxClippingPlanes) == 9);

struct ViewportOffset : CLPacket
{
    ViewportOffset(f32 x, f32 y, u32 coarseX, u32 coarseY) :
        CLPacket(OP_VIEWPORT_OFFSET),
        viewport_centre_x_coordinate(x * 256.f),
        // TODO: round
        // viewport_centre_x_coordinate(std::llroundf(x * 256.f)),
        coarse_x(coarseX),
        viewport_centre_y_coordinate(y * 256.f),
        // TODO: round
        // viewport_centre_y_coordinate(std::llroundf(y * 256.f)),
        coarse_y(coarseY)
    {
    }

    u32 viewport_centre_x_coordinate : 22; // fp 14.8
    u32 coarse_x : 10;
    u32 viewport_centre_y_coordinate : 22; // fp 14.8
    u32 coarse_y : 10;
} PACKED;
static_assert(sizeof(ViewportOffset) == 9);

struct ColorWriteMasks : CLPacket
{
    ColorWriteMasks(u32 mask) : CLPacket(OP_COLOR_WRITE_MASKS), mask(mask) {}
    u32 mask;
} PACKED;
static_assert(sizeof(ColorWriteMasks) == 5);

struct BlendConstantColor : CLPacket
{
    BlendConstantColor(u16 r, u16 g, u16 b, u16 a) :
        CLPacket(OP_BLEND_CONSTANT_COLOR),
        red_f16(r),
        green_f16(g),
        blue_f16(b),
        alpha_f16(a)
    {
    }

    u16 red_f16;
    u16 green_f16;
    u16 blue_f16;
    u16 alpha_f16;
} PACKED;
static_assert(sizeof(BlendConstantColor) == 9);

struct TransformFeedbackSpecs : CLPacket
{
    TransformFeedbackSpecs(u8 count, bool enable) :
        CLPacket(OP_TRANSFORM_FEEDBACK_SPECS),
        number_of_16_bit_output_data_specs_following(count),
        enable(enable)
    {
    }

    u8 number_of_16_bit_output_data_specs_following : 5;
    UNUSED_BITS(u8, 2);
    u8 enable : 1;
} PACKED;
static_assert(sizeof(TransformFeedbackSpecs) == 2);

struct SampleState : CLPacket
{
    SampleState(u8 mask, f32 coverage) :
        CLPacket(OP_SAMPLE_STATE),
        mask(mask),
        coverage(std::bit_cast<u32>(coverage) >> 16)
    {
    }

    u8 mask : 4;
    UNUSED_BITS(u8, 4);
    UNUSED_BITS(u8, 8);
    u16 coverage;
} PACKED;
static_assert(sizeof(SampleState) == 5);

struct VcmCacheSize : CLPacket
{
    VcmCacheSize(u8 binCount, u8 renderCount) :
        CLPacket(OP_VCM_CACHE_SIZE),
        number_of_16_vertex_batches_for_binning(binCount),
        number_of_16_vertex_batches_for_rendering(renderCount)
    {
    }

    u8 number_of_16_vertex_batches_for_binning : 4;
    u8 number_of_16_vertex_batches_for_rendering : 4;
} PACKED;
static_assert(sizeof(VcmCacheSize) == 2);

struct GlShaderState : CLPacket
{
    GlShaderState(u32 addr, u8 attrCount) :
        CLPacket(OP_GL_SHADER_STATE),
        number_of_attribute_arrays(attrCount),
        address(addr >> 4)
    {
        assert((addr & 0xf) == 0);
    }
    u32 number_of_attribute_arrays : 4;
    u32 address : 28;
} PACKED;
static_assert(sizeof(GlShaderState) == 5);

struct VertexArrayPrims : CLPacket
{
    VertexArrayPrims(u8 mode, u32 len, u32 first) :
        CLPacket(OP_VERTEX_ARRAY_PRIMS),
        mode(mode),
        length(len),
        index_of_first_vertex(first)
    {
    }
    // 4 = triangle
    u8 mode;
    u32 length;
    u32 index_of_first_vertex;
} PACKED;
static_assert(sizeof(VertexArrayPrims) == 10);

struct GLShaderStateRecord
{
    u8 point_size_in_shaded_vertex_data : 1;
    u8 enable_clipping : 1;
    u8 vertex_id_read_by_coordinate_shader : 1;
    u8 instance_id_read_by_coordinate_shader : 1;
    u8 base_instance_id_read_by_coordinate_shader : 1;
    u8 vertex_id_read_by_vertex_shader : 1;
    u8 instance_id_read_by_vertex_shader : 1;
    u8 base_instance_id_read_by_vertex_shader : 1;

    u8 fragment_shader_does_z_writes : 1;
    u8 turn_off_early_z_test : 1;
    u8 coordinate_shader_has_separate_input_and_output_vpm_blocks : 1;
    u8 vertex_shader_has_separate_input_and_output_vpm_blocks : 1;
    u8 fragment_shader_uses_real_pixel_centre_w_in_addition_to_centroid_w2 : 1;
    u8 enable_sample_rate_shading : 1;
    u8 any_shader_reads_hardware_written_primitive_id : 1;
    u8 insert_primitive_id_as_first_varying_to_fragment_shader : 1;

    u8 turn_off_scoreboard : 1;
    u8 do_scoreboard_wait_on_first_thread_switch : 1;
    u8 disable_implicit_point_line_varyings : 1;
    u8 no_prim_pack : 1;
    UNUSED_BITS(u8, 4);

    u8 number_of_varyings_in_fragment_shader;
    u8 coordinate_shader_output_vpm_segment_size : 4;
    u8 min_coord_shader_output_segments_required_in_play_in_addition_to_vcm_cache_size : 4;
    u8 coordinate_shader_input_vpm_segment_size : 4;
    u8 min_coord_shader_input_segments_required_in_play : 4; // -1
    u8 vertex_shader_output_vpm_segment_size : 4;
    u8 min_vertex_shader_output_segments_required_in_play_in_addition_to_vcm_cache_size : 4;
    u8 vertex_shader_input_vpm_segment_size : 4;
    u8 min_vertex_shader_input_segments_required_in_play : 4; // -1

    u32 address_of_default_attribute_values;

    u32 fragment_shader_4_way_threadable : 1;
    u32 fragment_shader_start_in_final_thread_section : 1;
    u32 fragment_shader_propagate_nans : 1;
    u32 fragment_shader_code_address : 29;

    u32 fragment_shader_uniforms_address;

    u32 vertex_shader_4_way_threadable : 1;
    u32 vertex_shader_start_in_final_thread_section : 1;
    u32 vertex_shader_propagate_nans : 1;
    u32 vertex_shader_code_address : 29;

    u32 vertex_shader_uniforms_address;

    u32 coordinate_shader_4_way_threadable : 1;
    u32 coordinate_shader_start_in_final_thread_section : 1;
    u32 coordinate_shader_propagate_nans : 1;
    u32 coordinate_shader_code_address : 29;

    u32 coordinate_shader_uniforms_address;
} PACKED;
static_assert(sizeof(GLShaderStateRecord) == 36);

struct GlShaderStateAttributeRecord
{
    u32 address;
    u8 vec_size : 2;
    u8 type : 3;
    u8 signed_int_type : 1;
    u8 normalized_int_type : 1;
    u8 read_as_int_uint : 1;
    u8 number_of_values_read_by_coordinate_shader : 4;
    u8 number_of_values_read_by_vertex_shader : 4;
    u16 instance_divisor;
    u32 stride;
    u32 maximum_index;
} PACKED;
static_assert(sizeof(GlShaderStateAttributeRecord) == 16);

enum v3d_tiling_mode
{
    /* Untiled resources.  Not valid as texture inputs. */
    V3D_TILING_RASTER,

    /* Single line of u-tiles. */
    V3D_TILING_LINEARTILE,

    /* Departure from standard 4-UIF block column format. */
    V3D_TILING_UBLINEAR_1_COLUMN,

    /* Departure from standard 4-UIF block column format. */
    V3D_TILING_UBLINEAR_2_COLUMN,

    /* Normal tiling format: grouped in 4x4 UIFblocks, each of which is
     * split 2x2 into utiles.
     */
    V3D_TILING_UIF_NO_XOR,

    /* Normal tiling format: grouped in 4x4 UIFblocks, each of which is
     * split 2x2 into utiles.
     */
    V3D_TILING_UIF_XOR,
};
enum V3D41_Decimate_Mode
{
    V3D_DECIMATE_MODE_SAMPLE_0 = 0,
    V3D_DECIMATE_MODE_4X = 1,
    V3D_DECIMATE_MODE_ALL_SAMPLES = 3,
};

enum V3D41_Output_Image_Format
{
    V3D_OUTPUT_IMAGE_FORMAT_SRGB8_ALPHA8 = 0,
    V3D_OUTPUT_IMAGE_FORMAT_SRGB = 1,
    V3D_OUTPUT_IMAGE_FORMAT_RGB10_A2UI = 2,
    V3D_OUTPUT_IMAGE_FORMAT_RGB10_A2 = 3,
    V3D_OUTPUT_IMAGE_FORMAT_ABGR1555 = 4,
    V3D_OUTPUT_IMAGE_FORMAT_ALPHA_MASKED_ABGR1555 = 5,
    V3D_OUTPUT_IMAGE_FORMAT_ABGR4444 = 6,
    V3D_OUTPUT_IMAGE_FORMAT_BGR565 = 7,
    V3D_OUTPUT_IMAGE_FORMAT_R11F_G11F_B10F = 8,
    V3D_OUTPUT_IMAGE_FORMAT_RGBA32F = 9,
    V3D_OUTPUT_IMAGE_FORMAT_RG32F = 10,
    V3D_OUTPUT_IMAGE_FORMAT_R32F = 11,
    V3D_OUTPUT_IMAGE_FORMAT_RGBA32I = 12,
    V3D_OUTPUT_IMAGE_FORMAT_RG32I = 13,
    V3D_OUTPUT_IMAGE_FORMAT_R32I = 14,
    V3D_OUTPUT_IMAGE_FORMAT_RGBA32UI = 15,
    V3D_OUTPUT_IMAGE_FORMAT_RG32UI = 16,
    V3D_OUTPUT_IMAGE_FORMAT_R32UI = 17,
    V3D_OUTPUT_IMAGE_FORMAT_RGBA16F = 18,
    V3D_OUTPUT_IMAGE_FORMAT_RG16F = 19,
    V3D_OUTPUT_IMAGE_FORMAT_R16F = 20,
    V3D_OUTPUT_IMAGE_FORMAT_RGBA16I = 21,
    V3D_OUTPUT_IMAGE_FORMAT_RG16I = 22,
    V3D_OUTPUT_IMAGE_FORMAT_R16I = 23,
    V3D_OUTPUT_IMAGE_FORMAT_RGBA16UI = 24,
    V3D_OUTPUT_IMAGE_FORMAT_RG16UI = 25,
    V3D_OUTPUT_IMAGE_FORMAT_R16UI = 26,
    V3D_OUTPUT_IMAGE_FORMAT_RGBA8 = 27,
    V3D_OUTPUT_IMAGE_FORMAT_RGB8 = 28,
    V3D_OUTPUT_IMAGE_FORMAT_RG8 = 29,
    V3D_OUTPUT_IMAGE_FORMAT_R8 = 30,
    V3D_OUTPUT_IMAGE_FORMAT_RGBA8I = 31,
    V3D_OUTPUT_IMAGE_FORMAT_RG8I = 32,
    V3D_OUTPUT_IMAGE_FORMAT_R8I = 33,
    V3D_OUTPUT_IMAGE_FORMAT_RGBA8UI = 34,
    V3D_OUTPUT_IMAGE_FORMAT_RG8UI = 35,
    V3D_OUTPUT_IMAGE_FORMAT_R8UI = 36,
    V3D_OUTPUT_IMAGE_FORMAT_BSTC = 39,
    V3D_OUTPUT_IMAGE_FORMAT_D32F = 40,
    V3D_OUTPUT_IMAGE_FORMAT_D24 = 41,
    V3D_OUTPUT_IMAGE_FORMAT_D16 = 42,
    V3D_OUTPUT_IMAGE_FORMAT_D24S8 = 43,
    V3D_OUTPUT_IMAGE_FORMAT_S8 = 44,
    V3D_OUTPUT_IMAGE_FORMAT_RGBA5551 = 45,
};

enum V3D41_Dither_Mode
{
    V3D_DITHER_MODE_NONE = 0,
    V3D_DITHER_MODE_RGB = 1,
    V3D_DITHER_MODE_A = 2,
    V3D_DITHER_MODE_RGBA = 3,
};
