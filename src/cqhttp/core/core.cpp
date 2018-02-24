#include "./core.h"

using namespace std;

namespace cqhttp {
    static vector<function<void()>> callback_initializers;

    static bool add_callback_initializer(const function<void()> initializer) {
        callback_initializers.push_back(initializer);
        return true;
    }

    static shared_ptr<Application> app;

    void apply(const shared_ptr<Application> app) {
        cqhttp::app = app;
        for (const auto &initializer : callback_initializers) {
            initializer();
        }
    }

    #define APP(FuncName, ...) \
        if (app) { app->FuncName(__VA_ARGS__); }

    #define LIFECYCLE(Name, ...) \
        static void __on_##Name(__VA_ARGS__) { APP(on_##Name); } \
        static bool __dummy_on_##Name = add_callback_initializer([]() { \
            cq::app::on_##Name = __on_##Name; \
        })

    LIFECYCLE(initialize);
    LIFECYCLE(enable);
    LIFECYCLE(disable);
    LIFECYCLE(coolq_start);
    LIFECYCLE(coolq_exit);

    #define EVENT(Name, ...) \
        static void __##Name##_event(__VA_ARGS__); \
        static bool __dummy_##Name##_event = add_callback_initializer([]() { \
            cq::event::Name = __##Name##_event; \
        }); \
        static void __##Name##_event(__VA_ARGS__)

    EVENT(on_private_msg, const cq::PrivateMessageEvent &e) {
        json data;
        APP(on_message_event, e.message_type, e.sub_type, data);
    }

    EVENT(on_group_msg, const cq::GroupMessageEvent &e) {
        json data;
        APP(on_message_event, e.message_type, e.sub_type, data);
    }

    EVENT(on_discuss_msg, const cq::DiscussMessageEvent &e) {
        json data;
        APP(on_message_event, e.message_type, e.sub_type, data);
    }

    EVENT(on_group_upload, const cq::GroupUploadEvent &e) {
        json data;
        APP(on_notice_event, e.notice_type, e.sub_type, data);
    }

    EVENT(on_group_admin, const cq::GroupAdminEvent &e) {
        json data;
        APP(on_notice_event, e.notice_type, e.sub_type, data);
    }

    EVENT(on_group_member_decrease, const cq::GroupMemberDecreaseEvent &e) {
        json data;
        APP(on_notice_event, e.notice_type, e.sub_type, data);
    }

    EVENT(on_group_member_increase, const cq::GroupMemberIncreaseEvent &e) {
        json data;
        APP(on_notice_event, e.notice_type, e.sub_type, data);
    }

    EVENT(on_friend_add, const cq::FriendAddEvent &e) {
        json data;
        APP(on_notice_event, e.notice_type, e.sub_type, data);
    }

    EVENT(on_friend_request, const cq::FriendRequestEvent &e) {
        json data;
        APP(on_request_event, e.request_type, e.sub_type, data);
    }

    EVENT(on_group_request, const cq::GroupRequestEvent &e) {
        json data;
        APP(on_request_event, e.request_type, e.sub_type, data);
    }
}
