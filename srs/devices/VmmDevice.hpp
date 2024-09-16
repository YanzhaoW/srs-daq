#pragma once

#include <cstdint>
#include <memory>
#include <vector>

namespace srs::vmm
{
    struct DeviceChannel
    {
        uint8_t sc{};
        uint8_t sl{};
        uint8_t st{};
        uint8_t sth{};
        uint8_t sm{};
        uint8_t smx{};
        uint8_t sd{};
        uint8_t sz10b{};
        uint8_t sz08b{};
        uint8_t sz06b{};
    };

    struct DeviceConfigSPI
    {
        uint8_t nskipm_i{};
        uint8_t sL0cktest{};
        uint8_t sL0dckinv{};
        uint8_t sL0ckinv{};
        uint8_t sL0ena{};
        uint8_t truncate{};
        uint8_t nskip{};
        uint8_t window{};
        uint16_t rollover{};
        uint16_t l0offset{};
        uint16_t offset{};
        /* global SPI 0 */
        uint8_t slvs{};
        uint8_t s32{};
        uint8_t stcr{};
        uint8_t ssart{};
        uint8_t srec{};
        uint8_t stlc{};
        uint8_t sbip{};
        uint8_t srat{};
        uint8_t sfrst{};
        uint8_t slvsbc{};
        uint8_t slvstp{};
        uint8_t slvstk{};
        uint8_t slvsdt{};
        uint8_t slvsart{};
        uint8_t slvstki{};
        uint8_t slvsena{};
        uint8_t slvs6b{};
        uint8_t sL0enaV{};
        uint8_t reset1{};
        uint8_t reset2{};
        /* SPI 1 */
        uint16_t sdt{};
        uint16_t sdp10{};
        uint8_t sc10b{};
        uint8_t sc8b{};
        uint8_t sc6b{};
        uint8_t s8b{};
        uint8_t s6b{};
        uint8_t s10b{};
        uint8_t sdcks{};
        uint8_t sdcka{};
        uint8_t sdck6b{};
        uint8_t sdrv{};
        uint8_t stpp{};
        /* SPI 2 */
        uint8_t sp{};
        uint8_t sdp{};
        uint8_t sbmx{};
        uint8_t sbft{};
        uint8_t sbfp{};
        uint8_t sbfm{};
        uint8_t slg{};
        uint8_t sm5_sm0{};
        uint8_t scmx{};
        uint8_t sfa{};
        uint8_t sfam{};
        uint8_t st{};
        uint8_t sfm{};
        uint8_t sg{};
        uint8_t sng{};
        uint8_t stot{};
        uint8_t sttt{};
        uint8_t ssh{};
        uint8_t stc{};
    };

    struct DeviceConfig
    {
        std::unique_ptr<DeviceConfigSPI> spi_config = std::make_unique<DeviceConfigSPI>();
        std::vector<DeviceChannel> channels;
    };

    class Device
    {
        std::vector<DeviceConfig> configs;
    };
} // namespace srs::vmm
