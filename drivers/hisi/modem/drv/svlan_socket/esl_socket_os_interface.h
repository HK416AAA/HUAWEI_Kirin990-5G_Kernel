
#include <linux/sched.h>
#include <linux/atomic.h>
#include <linux/mutex.h>
#include <linux/net.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/wait.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>

#include <osl_bio.h>
#include <osl_sem.h>

#define SIM_IRQ_HANDLED IRQ_HANDLED
#define SIM_DMA_FROM_DEVICE DMA_FROM_DEVICE
#define SIM_DMA_TO_DEVICE DMA_TO_DEVICE
#define SIM_IRQF_SHARED IRQF_SHARED

typedef dma_addr_t sim_dma_addr_t;
typedef enum dma_data_direction sim_dma_data_direction;
typedef struct mutex sim_mutex;
typedef atomic_t sim_atomic_t;
typedef irqreturn_t sim_irqreturn_t;
typedef struct platform_device sim_platform_device;
typedef irq_handler_t sim_irq_handler_t;

unsigned int simsock_readl(const void *addr);

void simsock_writel(unsigned int val, void *addr);

void sim_sleep(int sec);

sim_dma_addr_t sim_dma_map_single(struct device *dev, void *dam_handler, size_t size, sim_dma_data_direction direction);

void sim_dma_unmap_single(struct device *dev, sim_dma_addr_t dma_handle, size_t size, sim_dma_data_direction direction);

void sim_mutex_lock(sim_mutex *m_lock);

void sim_mutex_unlock(sim_mutex *m_unlock);

int sim_atomic_read(sim_atomic_t *ato_t);

void sim_atomic_set(sim_atomic_t *ato_t, int n);

int sim_atomic_inc_not_zero(sim_atomic_t *ato_t);

int sim_atomic_dec_and_test(sim_atomic_t *ato_t);

void sim_mutex_init(sim_mutex *m_init);

int sim_request_irq(unsigned int irq, sim_irq_handler_t handler, unsigned long flags, const char *name, void *dev);
