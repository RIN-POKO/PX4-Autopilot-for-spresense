

#ifndef RTT_ACK_HPP
#define RTT_ACK_HPP

#include <uORB/topics/rtt_ack.h>

class MavlinkStreamRTTACK : public MavlinkStream
{
public:
	static MavlinkStream *new_instance(Mavlink *mavlink) { return new MavlinkStreamRTTACK(mavlink); }

	static constexpr const char *get_name_static() { return "RTT_ACK"; }
	static constexpr uint16_t get_id_static() { return MAVLINK_MSG_ID_RTT_ACK; }

	const char *get_name() const override { return get_name_static(); }
	uint16_t get_id() override { return get_id_static(); }

	unsigned get_size() override
	{
		return _rtt_ack_sub.advertised() ? (MAVLINK_MSG_ID_RTT_ACK_LEN + MAVLINK_NUM_NON_PAYLOAD_BYTES) : 0;
	}

private:
	explicit MavlinkStreamRTTACK(Mavlink *mavlink) : MavlinkStream(mavlink) {}

	uORB::Subscription _rtt_ack_sub{ORB_ID(rtt_ack), 0};

	bool send() override
	{
		rtt_ack_s rtt_ack;

		if (_rtt_ack_sub.update(&rtt_ack)) {
			mavlink_rtt_ack_t msg{};

			msg.time_usec = rtt_ack.timestamp;

			mavlink_msg_rtt_ack_send_struct(_mavlink->get_channel(), &msg);

			return true;
		}

		return false;
	}
};

#endif // RTT_ACK_HPP
