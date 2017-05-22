/*
 * Copyright (c) 2007, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 * Jiang Yunhong <yunhong.jiang@intel.com>
 *
 * This file implements direct PCI assignment to a HVM guest
 */

#include "pt-msi.h"
#include <sys/mman.h>

void msi_set_enable(struct pt_dev *dev, int en)
{
    uint16_t val = 0;
    uint32_t address = 0;
    if (!dev->msi)
        return;

    address = dev->msi->ctrl_offset;
    if (!address)
        return;

    val = pci_read_word(dev->pci_dev, address);
    val &= ~PCI_MSI_FLAGS_ENABLE;
    val |= en & PCI_MSI_FLAGS_ENABLE;
    pci_write_word(dev->pci_dev, address, val);
}

static void msix_set_enable(struct pt_dev *dev, int en)
{
    uint16_t val = 0;
    uint32_t address = 0;
    if (!dev->msix)
        return;

    address = dev->msix->ctrl_offset;
    if (!address)
        return;

    val = pci_read_word(dev->pci_dev, address);
    val &= ~PCI_MSIX_ENABLE;
    if (en)
        val |= PCI_MSIX_ENABLE;
    pci_write_word(dev->pci_dev, address, val);
}

/* MSI virtuailization functions */

/*
 * setup physical msi, but didn't enable it
 */
int pt_msi_setup(struct pt_dev *dev)
{
    int pirq = -1;
    uint8_t gvec = 0;

    if ( !(dev->msi->flags & MSI_FLAG_UNINIT) )
    {
        PT_LOG("Error: setup physical after initialized?? \n");
        return -1;
    }

    gvec = dev->msi->data & 0xFF;
    if (!gvec) {
        /* if gvec is 0, the guest is asking for a particular pirq that
         * is passed as dest_id */
        pirq = (dev->msi->addr_hi & 0xffffff00) |
               ((dev->msi->addr_lo >> MSI_TARGET_CPU_SHIFT) & 0xff);
        if (!pirq)
            /* this probably identifies an misconfiguration of the guest,
             * try the emulated path */
            pirq = -1;
        else
            PT_LOG("pt_msi_setup requested pirq = %d\n", pirq);
    }

    if ( xc_physdev_map_pirq_msi(xc_handle, domid, AUTO_ASSIGN, &pirq,
                                 PCI_DEVFN(dev->pci_dev->dev,
                                           dev->pci_dev->func),
                                 dev->pci_dev->bus, 0, 0) )
    {
        PT_LOG("Error: Mapping of MSI failed.\n");
        return -1;
    }

    if ( pirq < 0 )
    {
        PT_LOG("Error: Invalid pirq number\n");
        return -1;
    }

    dev->msi->pirq = pirq;
    PT_LOG("msi mapped with pirq %x\n", pirq);

    return 0;
}

uint32_t __get_msi_gflags(uint32_t data, uint64_t addr)
{
    uint32_t result = 0;
    int rh, dm, dest_id, deliv_mode, trig_mode;

    rh = (addr >> MSI_ADDR_REDIRECTION_SHIFT) & 0x1;
    dm = (addr >> MSI_ADDR_DESTMODE_SHIFT) & 0x1;
    dest_id = (addr >> MSI_TARGET_CPU_SHIFT) & 0xff;
    deliv_mode = (data >> MSI_DATA_DELIVERY_SHIFT) & 0x7;
    trig_mode = (data >> MSI_DATA_TRIGGER_SHIFT) & 0x1;

    result |= dest_id | (rh << GFLAGS_SHIFT_RH) | (dm << GFLAGS_SHIFT_DM) | \
                (deliv_mode << GLFAGS_SHIFT_DELIV_MODE) |
                (trig_mode << GLFAGS_SHIFT_TRG_MODE);

    return result;
}

int pt_msi_update(struct pt_dev *d)
{
    uint8_t gvec = 0;
    uint32_t gflags = 0;
    uint64_t addr = 0;
    int ret = 0;

    /* get vector, address, flags info, etc. */
    gvec = d->msi->data & 0xFF;
    addr = (uint64_t)d->msi->addr_hi << 32 | d->msi->addr_lo;
    gflags = __get_msi_gflags(d->msi->data, addr);

    PT_LOG("Update msi with pirq %x gvec %x gflags %x\n",
           d->msi->pirq, gvec, gflags);

    ret = xc_domain_update_msi_irq(xc_handle, domid, gvec,
                                     d->msi->pirq, gflags, 0);

    if (ret)
    {
        PT_LOG("Error: Binding of MSI failed.\n");

        if (xc_physdev_unmap_pirq(xc_handle, domid, d->msi->pirq))
            PT_LOG("Error: Unmapping of MSI failed.\n");
        d->msi->pirq = -1;
        return ret;
    }
    return 0;
}

void pt_msi_disable(struct pt_dev *dev)
{
    PCIDevice *d = &dev->dev;
    uint8_t gvec = 0;
    uint32_t gflags = 0;
    uint64_t addr = 0;
    uint8_t e_device = 0;
    uint8_t e_intx = 0;

    msi_set_enable(dev, 0);

    e_device = PCI_SLOT(dev->dev.devfn);
    e_intx = pci_intx(dev);

    if (dev->msi_trans_en)
    {
        if (xc_domain_unbind_pt_irq(xc_handle, domid, dev->msi->pirq,
                                    PT_IRQ_TYPE_MSI_TRANSLATE, 0,
                                    e_device, e_intx, 0))
        {
            PT_LOG("Error: Unbinding pt irq for MSI-INTx failed!\n");
            goto out;
        }
    }
    else if (!(dev->msi->flags & MSI_FLAG_UNINIT))
    {
        /* get vector, address, flags info, etc. */
        gvec = dev->msi->data & 0xFF;
        addr = (uint64_t)dev->msi->addr_hi << 32 | dev->msi->addr_lo;
        gflags = __get_msi_gflags(dev->msi->data, addr);

        PT_LOG("Unbind msi with pirq %x, gvec %x\n",
                dev->msi->pirq, gvec);

        if (xc_domain_unbind_msi_irq(xc_handle, domid, gvec,
                                        dev->msi->pirq, gflags))
        {
            PT_LOG("Error: Unbinding of MSI failed. [%02x:%02x.%x]\n",
                pci_bus_num(d->bus), PCI_SLOT(d->devfn), PCI_FUNC(d->devfn));
            goto out;
        }
    }

    if (dev->msi->pirq != -1)
    {
        PT_LOG("Unmap msi with pirq %x\n", dev->msi->pirq);

        if (xc_physdev_unmap_pirq(xc_handle, domid, dev->msi->pirq))
        {
            PT_LOG("Error: Unmapping of MSI failed. [%02x:%02x.%x]\n",
               pci_bus_num(d->bus), PCI_SLOT(d->devfn), PCI_FUNC(d->devfn));
            goto out;
        }
    }

out:
    /* clear msi info */
    dev->msi->flags &= ~(PT_MSI_MAPPED | PCI_MSI_FLAGS_ENABLE);
    dev->msi->flags |= MSI_FLAG_UNINIT;
    dev->msi->pirq = -1;
    dev->msi_trans_en = 0;
}

/* MSI-INTx translation virtulization functions */
int pt_enable_msi_translate(struct pt_dev* dev)
{
    uint8_t e_device = 0;
    uint8_t e_intx = 0;

    if (!(dev->msi && dev->msi_trans_cap))
        return -1;

    msi_set_enable(dev, 0);
    dev->msi_trans_en = 0;

    if (pt_msi_setup(dev))
    {
        PT_LOG("Error: MSI-INTx translation MSI setup failed, fallback\n");
        return -1;
    }

    e_device = PCI_SLOT(dev->dev.devfn);
    /* fix virtual interrupt pin to INTA# */
    e_intx = pci_intx(dev);

    if (xc_domain_bind_pt_irq(xc_handle, domid, dev->msi->pirq,
                               PT_IRQ_TYPE_MSI_TRANSLATE, 0,
                               e_device, e_intx, 0))
    {
        PT_LOG("Error: MSI-INTx translation bind failed, fallback\n");

        if (xc_physdev_unmap_pirq(xc_handle, domid, dev->msi->pirq))
            PT_LOG("Error: Unmapping of MSI failed.\n");
        dev->msi->pirq = -1;
        return -1;
    }

    msi_set_enable(dev, 1);
    dev->msi_trans_en = 1;

    return 0;
}

void pt_disable_msi_translate(struct pt_dev *dev)
{
    uint8_t e_device = 0;
    uint8_t e_intx = 0;

    pt_msi_disable(dev);
    dev->msi->flags |= MSI_FLAG_UNINIT;

    if (dev->machine_irq)
    {
        if (xc_domain_bind_pt_pci_irq(xc_handle, domid, dev->machine_irq,
                                       0, e_device, e_intx))
            PT_LOG("Error: Rebinding of interrupt failed!\n");
    }
}

static int pt_msix_update_one(struct pt_dev *dev, int entry_nr)
{
    struct msix_entry_info *entry = &dev->msix->msix_entry[entry_nr];
    int pirq = entry->pirq;
    int gvec = entry->io_mem[2] & 0xff;
    uint64_t gaddr = *(uint64_t *)&entry->io_mem[0];
    uint32_t gflags = __get_msi_gflags(entry->io_mem[2], gaddr);
    int ret;

    if ( !entry->flags )
        return 0;

    if (!gvec) {
        /* if gvec is 0, the guest is asking for a particular pirq that
         * is passed as dest_id */
        pirq = ((gaddr >> 32) & 0xffffff00) |
               (((gaddr & 0xffffffff) >> MSI_TARGET_CPU_SHIFT) & 0xff);
        if (!pirq)
            /* this probably identifies an misconfiguration of the guest,
             * try the emulated path */
            pirq = -1;
        else
            PT_LOG("pt_msix_update_one requested pirq = %d\n", pirq);
    }

    /* Check if this entry is already mapped */
    if ( entry->pirq == -1 )
    {
        ret = xc_physdev_map_pirq_msi(xc_handle, domid, AUTO_ASSIGN, &pirq,
                                PCI_DEVFN(dev->pci_dev->dev,
                                          dev->pci_dev->func),
                                dev->pci_dev->bus, entry_nr,
                                dev->msix->table_base);
        if ( ret )
        {
            PT_LOG("Error: Mapping msix entry %x\n", entry_nr);
            return ret;
        }
        entry->pirq = pirq;
    }

    PT_LOG("Update msix entry %x with pirq %x gvec %x\n",
            entry_nr, pirq, gvec);

    ret = xc_domain_update_msi_irq(xc_handle, domid, gvec, pirq, gflags,
                                   dev->msix->mmio_base_addr);
    if ( ret )
    {
        PT_LOG("Error: Updating msix irq info for entry %d\n", entry_nr);

        if (xc_physdev_unmap_pirq(xc_handle, domid, entry->pirq))
            PT_LOG("Error: Unmapping of MSI-X failed.\n");
        entry->pirq = -1;
        return ret;
    }

    entry->flags = 0;

    return 0;
}

int pt_msix_update(struct pt_dev *dev)
{
    struct pt_msix_info *msix = dev->msix;
    int i;

    for ( i = 0; i < msix->total_entries; i++ )
    {
        pt_msix_update_one(dev, i);
    }

    return 0;
}

void pt_msix_disable(struct pt_dev *dev)
{
    PCIDevice *d = &dev->dev;
    uint8_t gvec = 0;
    uint32_t gflags = 0;
    uint64_t addr = 0;
    int i = 0;
    struct msix_entry_info *entry = NULL;

    msix_set_enable(dev, 0);

    for ( i = 0; i < dev->msix->total_entries; i++ )
    {
        entry = &dev->msix->msix_entry[i];

        if (entry->pirq == -1)
            continue;

        gvec = entry->io_mem[2] & 0xff;
        addr = *(uint64_t *)&entry->io_mem[0];
        gflags = __get_msi_gflags(entry->io_mem[2], addr);

        PT_LOG("Unbind msix with pirq %x, gvec %x\n",
                entry->pirq, gvec);

        if (xc_domain_unbind_msi_irq(xc_handle, domid, gvec,
                                        entry->pirq, gflags))
            PT_LOG("Error: Unbinding of MSI-X failed. [%02x:%02x.%x]\n",
                pci_bus_num(d->bus), PCI_SLOT(d->devfn), PCI_FUNC(d->devfn));
        else
        {
            PT_LOG("Unmap msix with pirq %x\n", entry->pirq);

            if (xc_physdev_unmap_pirq(xc_handle,
                                         domid, entry->pirq))
                PT_LOG("Error: Unmapping of MSI-X failed. [%02x:%02x.%x]\n",
                    pci_bus_num(d->bus),
                    PCI_SLOT(d->devfn), PCI_FUNC(d->devfn));
        }
        /* clear msi-x info */
        entry->pirq = -1;
        entry->flags = 0;
    }
}

int pt_msix_update_remap(struct pt_dev *dev, int bar_index)
{
    struct msix_entry_info *entry;
    int i, ret;

    if ( !(dev->msix && dev->msix->bar_index == bar_index) )
        return 0;

    for ( i = 0; i < dev->msix->total_entries; i++ )
    {
        entry = &dev->msix->msix_entry[i];
        if ( entry->pirq != -1 )
        {
            ret = xc_domain_unbind_pt_irq(xc_handle, domid, entry->pirq,
                                          PT_IRQ_TYPE_MSI, 0, 0, 0, 0);
            if ( ret )
                PT_LOG("Error: unbind MSI-X entry %d failed\n", entry->pirq);
            entry->flags = 1;
        }
    }
    pt_msix_update(dev);

    return 0;
}

static void pci_msix_invalid_write(void *opaque, target_phys_addr_t addr,
                                   uint32_t val)
{
    PT_LOG("Error: Invalid write to MSI-X table, addr %016"PRIx64";"
           " only dword access is allowed\n", addr);
}

static void pci_msix_writel(void *opaque, target_phys_addr_t addr, uint32_t val)
{
    struct pt_dev *dev = (struct pt_dev *)opaque;
    struct pt_msix_info *msix = dev->msix;
    struct msix_entry_info *entry;
    int entry_nr, offset;

    if ( addr % 4 )
    {
        PT_LOG("Error: Unaligned dword write to MSI-X table,"
               " addr %016"PRIx64"\n", addr);
        return;
    }

    entry_nr = (addr - msix->mmio_base_addr) / 16;
    entry = &msix->msix_entry[entry_nr];
    offset = ((addr - msix->mmio_base_addr) % 16) / 4;

    if ( offset != 3 )
    {
        const volatile uint32_t *vec_ctrl;

        if ( entry->io_mem[offset] == val && entry->pirq != -1)
            return;

        /*
         * If Xen intercepts the mask bit access, io_mem[3] may not be
         * up-to-date. Read from hardware directly.
         */
        vec_ctrl = msix->phys_iomem_base + 16 * entry_nr + 12;

        if ( msix->enabled && !(*vec_ctrl & 0x1) )
        {
            PT_LOG("Can't update entry %d since MSI-X is already enabled"
                   " (%08"PRIx32" -> %08"PRIx32")\n",
                   entry_nr, entry->io_mem[offset], val);
            return;
        }

        entry->flags = 1;
    }

    entry->io_mem[offset] = val;

    if ( offset == 3 )
    {
        if ( msix->enabled && !(val & 0x1) )
            pt_msix_update_one(dev, entry_nr);
    }
}

static CPUWriteMemoryFunc *pci_msix_write[] = {
    pci_msix_invalid_write,
    pci_msix_invalid_write,
    pci_msix_writel
};

static uint32_t pci_msix_invalid_read(void *opaque, target_phys_addr_t addr)
{
    PT_LOG("Error: Invalid read >from MSI-X table, addr %016"PRIx64";"
           " only dword access is allowed\n", addr);
    return 0;
}

static uint32_t pci_msix_readl(void *opaque, target_phys_addr_t addr)
{
    struct pt_dev *dev = (struct pt_dev *)opaque;
    struct pt_msix_info *msix = dev->msix;
    int entry_nr, offset;

    if ( addr % 4 )
    {
        PT_LOG("Error: Unaligned dword read from MSI-X table,"
               " addr %016"PRIx64"\n", addr);
        return 0;
    }

    entry_nr = (addr - msix->mmio_base_addr) / 16;
    offset = ((addr - msix->mmio_base_addr) % 16) / 4;

    if ( addr - msix->mmio_base_addr < msix->total_entries * 16 )
        return msix->msix_entry[entry_nr].io_mem[offset];
    else
        return *(uint32_t *)(msix->phys_iomem_base +
                             (addr - msix->mmio_base_addr));
}

static CPUReadMemoryFunc *pci_msix_read[] = {
    pci_msix_invalid_read,
    pci_msix_invalid_read,
    pci_msix_readl
};

int has_msix_mapping(struct pt_dev *dev, int bar_index)
{
    if ( !(dev->msix && dev->msix->bar_index == bar_index) )
        return 0;

    return 1;
}

int pt_msix_init(struct pt_dev *dev, int pos)
{
    uint8_t id;
    uint16_t control;
    int i, total_entries, table_off, bar_index;
    struct pci_dev *pd = dev->pci_dev;
    int fd;

    id = pci_read_byte(pd, pos + PCI_CAP_LIST_ID);

    if ( id != PCI_CAP_ID_MSIX )
    {
        PT_LOG("Error: Invalid id %x pos %x\n", id, pos);
        return -1;
    }

    control = pci_read_word(pd, pos + 2);
    total_entries = control & 0x7ff;
    total_entries += 1;

    dev->msix = malloc(sizeof(struct pt_msix_info)
                       + total_entries*sizeof(struct msix_entry_info));
    if ( !dev->msix )
    {
        PT_LOG("Error: Allocating pt_msix_info failed.\n");
        return -1;
    }
    memset(dev->msix, 0, sizeof(struct pt_msix_info)
                         + total_entries*sizeof(struct msix_entry_info));
    dev->msix->total_entries = total_entries;
    for ( i = 0; i < total_entries; i++ )
        dev->msix->msix_entry[i].pirq = -1;

    dev->msix->mmio_index =
        cpu_register_io_memory(0, pci_msix_read, pci_msix_write, dev);

    table_off = pci_read_long(pd, pos + PCI_MSIX_TABLE);
    bar_index = dev->msix->bar_index = table_off & PCI_MSIX_BIR;
    table_off = dev->msix->table_off = table_off & ~PCI_MSIX_BIR;
    dev->msix->table_base = pt_pci_base_addr(dev->pci_dev->base_addr[bar_index]);
    PT_LOG("get MSI-X table bar base %llx\n",
           (unsigned long long)dev->msix->table_base);

    fd = open("/dev/mem", O_RDWR);
    if ( fd == -1 )
    {
        PT_LOG("Error: Can't open /dev/mem: %s\n", strerror(errno));
        goto error_out;
    }
    PT_LOG("table_off = %x, total_entries = %d\n", table_off, total_entries);
    dev->msix->table_offset_adjust = table_off & 0x0fff;
    dev->msix->phys_iomem_base = mmap(0, total_entries * 16 + dev->msix->table_offset_adjust,
                          PROT_READ, MAP_SHARED | MAP_LOCKED,
                          fd, dev->msix->table_base + table_off - dev->msix->table_offset_adjust);
    close(fd);
    if ( dev->msix->phys_iomem_base == MAP_FAILED )
    {
        PT_LOG("Error: Can't map physical MSI-X table: %s\n", strerror(errno));
        goto error_out;
    }

    dev->msix->phys_iomem_base += dev->msix->table_offset_adjust;

    PT_LOG("mapping physical MSI-X table to %lx\n",
           (unsigned long)dev->msix->phys_iomem_base);
    return 0;

error_out:
    free(dev->msix);
    dev->msix = NULL;
    return -1;
}

void pt_msix_delete(struct pt_dev *dev)
{
    /* unmap the MSI-X memory mapped register area */
    if (dev->msix->phys_iomem_base)
    {
        PT_LOG("unmapping physical MSI-X table from %lx\n",
           (unsigned long)dev->msix->phys_iomem_base);
        munmap(dev->msix->phys_iomem_base, dev->msix->total_entries * 16 + 
           dev->msix->table_offset_adjust);
    }

    if (dev->msix->mmio_index > 0)
    {
        cpu_unregister_io_memory(dev->msix->mmio_index);
    }


    free(dev->msix);
    dev->msix = NULL;
}
