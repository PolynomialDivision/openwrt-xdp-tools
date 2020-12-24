/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/bpf.h>
#include <linux/in.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>
#include <xdp/xdp_helpers.h>
#include <xdp/parsing_helpers.h>

#ifndef XDP_ACTION_MAX
#define XDP_ACTION_MAX (XDP_REDIRECT + 1)
#endif

struct ip_stats_rec {
	__u64 ipv4_rx_packets;
	__u64 ipv4_rx_bytes;
	__u64 ipv6_rx_packets;
	__u64 ipv6_rx_bytes;
};

struct bpf_map_def SEC("maps") ip_stats_map = {
	.type        = BPF_MAP_TYPE_ARRAY,
	.key_size    = sizeof(__u32),
	.value_size  = sizeof(struct ip_stats_rec),
	.max_entries = XDP_ACTION_MAX,
};

SEC("xdp-ip-stats")
int ipv6_analyzer(struct xdp_md *ctx)
{
	void *data_end = (void *)(long)ctx->data_end;
	void *data     = (void *)(long)ctx->data;

	struct ethhdr *ehdr = data;
	if (ehdr + 1 > data_end)
		goto out;

	__u32 action = XDP_PASS;
	struct ip_stats_rec *rec = bpf_map_lookup_elem(&ip_stats_map, &action);
	if (!rec)
		goto out;

	__u64 bytes = data_end - data;

	if (bpf_ntohs(ehdr->h_proto) != ETH_P_IPV6) {
		rec->ipv6_rx_packets++;
		rec->ipv6_rx_bytes += bytes;
	} else {
		rec->ipv4_rx_packets++;
		rec->ipv4_rx_bytes += bytes;
	}

out:
	return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
