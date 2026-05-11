#pragma once

#include <concepts>
#include <ostream>

#include "data_getter.hpp"

namespace stream::image
{

template <typename Controler,
          typename ProxyTextureType,
          typename StagingTextureType>
concept Controlerable = requires(ProxyTextureType proxy,
                                 StagingTextureType staging,
                                 Controler cont,
                                 Data data) {
    { cont.updateDataOnGui(proxy) } -> std::convertible_to<bool>;
    { cont.fillStaging(proxy, staging) } -> std::convertible_to<bool>;
    { cont.shareData(staging, data) } -> std::convertible_to<bool>;
};
template <typename Controler, typename StagingTextureType>
concept ControlerableSolo =
    requires(StagingTextureType staging, Controler cont, Data data) {
        { cont.updateDataOnGui() } -> std::convertible_to<bool>;
        { cont.fillStaging(staging) } -> std::convertible_to<bool>;
        { cont.shareData(staging, data) } -> std::convertible_to<bool>;
    };

template <typename... Args>
struct ProxyViewInVideoBuffer;

template <typename StagingTextureType,
          ControlerableSolo<StagingTextureType> Controler>
struct ProxyViewInVideoBuffer<StagingTextureType, Controler>
{
public:
    ProxyViewInVideoBuffer(StagingTextureType& staging, Controler& controler)
        : m_staging(staging), m_controler(controler)
    {
    }

    bool updateDataOnGui()
    {
        return m_controler.updateDataOnGui();
    }

    bool fillStaging(Data& getter) noexcept
    {
        if (false == m_controler.fillStaging(m_staging))
        {
            return false;
        }
        shareData(getter);
        return true;
    }

    bool shareData(Data& getter) noexcept
    {
        return m_controler.shareData(m_staging, getter);
    }

private:
    StagingTextureType& m_staging;
    Controler& m_controler;
};

template <typename ProxyTextureType,
          typename StagingTextureType,
          Controlerable<ProxyTextureType, StagingTextureType> Controler>
struct ProxyViewInVideoBuffer<ProxyTextureType, StagingTextureType, Controler>
{
public:
    ProxyViewInVideoBuffer(ProxyTextureType& proxy,
                           StagingTextureType& staging,
                           Controler& controler)
        : m_proxy(proxy), m_staging(staging), m_controler(controler)
    {
    }

    bool updateDataOnGui()
    {
        return m_controler.updateDataOnGui(m_proxy);
    }

    bool fillStaging(Data& getter) noexcept
    {
        if (false == m_controler.fillStaging(m_proxy, m_staging))
        {
            return false;
        }
        shareData(getter);
        return true;
    }

    bool shareData(Data& getter) noexcept
    {
        return m_controler.shareData(m_staging, getter);
    }

private:
    ProxyTextureType& m_proxy;
    StagingTextureType& m_staging;
    Controler& m_controler;
};
} // namespace stream::image
