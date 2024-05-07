
#include "esl_socket_os_interface.h"

unsigned int simsock_readl(const void *addr)
{
    return readl(addr);
}

void simsock_writel(unsigned int val, void *addr)
{
    writel(val, addr);
}

void sim_sleep(int sec)
{
    msleep(sec);
}

sim_dma_addr_t sim_dma_map_single(struct device *dev, void *dam_handler, size_t size, sim_dma_data_direction direction)
{
    sim_dma_addr_t retval;
    retval = dma_map_single(dev, dam_handler, size, direction);
    return retval;
}

void sim_dma_unmap_single(struct device *dev, sim_dma_addr_t dma_handle, size_t size, sim_dma_data_direction direction)
{
    dma_unmap_single(dev, dma_handle, size, direction);
}

void sim_mutex_lock(sim_mutex *m_lock)
{
    mutex_lock(m_lock);
}

void sim_mutex_unlock(sim_mutex *m_unlock)
{
    mutex_unlock(m_unlock);
}

void sim_mutex_init(sim_mutex *m_init)
{
    mutex_init(m_init);
}

int sim_atomic_read(sim_atomic_t *ato_t)
{
    return atomic_read(ato_t);
}

void sim_atomic_set(sim_atomic_t *ato_t, int n)
{
    atomic_set(ato_t, n);
}

int sim_atomic_inc_not_zero(sim_atomic_t *ato_t)
{
    return atomic_inc_not_zero(ato_t);
}

int sim_atomic_dec_and_test(sim_atomic_t *ato_t)
{
    return atomic_dec_and_test(ato_t);
}

int sim_request_irq(unsigned int irq, sim_irq_handler_t handler, unsigned long flags, const char *name, void *dev)
{
    return request_irq(irq, handler, flags, name, dev);
}
