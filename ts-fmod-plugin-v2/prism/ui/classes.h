#pragma once

// Current ui types:
// ui::window - bearly
// ui::group - bearly
// ui::view - bearly
// ui::text
// ui::text_common

#pragma pack(push, 1)
namespace prism::ui
{
    class element_base_u : public unit_t
    {
    public:
        int32_t tab;            //0x0010 (0x04)
        int32_t id;             //0x0014 (0x04)
        float coords_l;         //0x0018 (0x04)
        float coords_r;         //0x001C (0x04)
        float coords_b;         //0x0020 (0x04)
        float coords_t;         //0x0024 (0x04)
        float area_l;           //0x0028 (0x04)
        float area_r;           //0x002C (0x04)
        float area_b;           //0x0030 (0x04)
        float area_t;           //0x0034 (0x04)
    };

    class view_u : public element_base_u        // Size: 0x0088
    {
    public:
        char pad_0038[24];              //0x0038 (0x18)
        void* parent;                   //0x0050 (0x08)
        void* window;                   //0x0058 (0x08)
        char pad_0060[16];              //0x0060 (0x10)
        void** my_children;             //0x0070 (0x08)
        uint64_t my_children_count;     //0x0078 (0x08)
        uint64_t my_children_max_size;  //0x0080 (0x08)

        /*
        void* get(int element_id)
        {
            if (this->id == element_id) return this;

            for (int i = 0; i < this->my_children_count; ++i) {
                prism::ui::element_base_u* child = (prism::ui::element_base_u*)this->my_children[i];

                if (child->id == element_id) return child;
                else
                {
                    if (std::string(child->get_unit_descriptor()->string->value) == "ui::group")
                    {
                        prism::ui::view_u* group = (prism::ui::view_u*)this->my_children[i];

                        for (int ii = 0; ii < group->my_children_count; ++ii) {
                            prism::ui::element_base_u* child_two = (prism::ui::element_base_u*)group->my_children[ii];
                            
                            if (child_two->id == element_id) return child_two;
                            else
                            {
                                if (std::string(child_two->get_unit_descriptor()->string->value) == "ui::group")
                                {
                                    prism::ui::view_u* group_two = (prism::ui::view_u*)this->my_children[ii];
                                    for (int iii = 0; iii < group_two->my_children_count; ++iii) {
                                        prism::ui::element_base_u* child_three = (prism::ui::element_base_u*)group_two->my_children[iii];
                                        if (child_three->id == element_id) return child_two;
                                        else return nullptr;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            return nullptr;
        }
        */

    };
    static_assert(sizeof(view_u) == 0x88);

    class group_u : public view_u {}; // Size: 0x0088
    static_assert(sizeof(group_u) == 0x88);

    class window_u : public group_u {}; // Size: 0x0088
    static_assert(sizeof(window_u) == 0x88);

    class text_u : public element_base_u // Size: 0x00DC
    {
    public:
        char pad_0038[20];      //0x0038 (0x14)
        void* parent;           //0x004C (0x08)
        window_u* window;       //0x0054 (0x08)
        char pad_005C[104];     //0x005C (0x68)
        string_dyn_t text;      //0x00C4 (0x18)
    };
    static_assert(sizeof(text_u) == 0x00DC);

    class text_common_u : public element_base_u // Size: 0x0110
    {
    public:
        char pad_0038[24];          //0x0038 (0x18)
        void* parent;               //0x0050 (0x08)
        window_u* window;           //0x0058 (0x08)
        char pad_0060[112];         //0x0060 (0x60)
        char* value_with_template;  //0x00D0 (0x08)
        char pad_00D8[24];          //0x00D8 (0x18)
        void* text_template;        //0x00F0 (0x08)
        class string_dyn_t value;   //0x00F8 (0x18)
    };
    static_assert(sizeof(text_common_u) == 0x110);
};
#pragma pack(pop)