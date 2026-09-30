
             // window creation with static inits
            {
                static bool showwindow = true;
                static bool state = false;
                static bool state_dark = false;
                static bool checkbox = false;  
                static int radiobutton = 0;

                static int counter = 0;

                if (showwindow)
                {
                    ImGui::Begin("My new custom Window", &showwindow, ImGuiWindowFlags_NoScrollbar);

                    if (ImGui::Button("Random##Light"))
                    {
                        if (state)
                        {
                            state = false;
                        }
                        else
                        {
                            state = true;
                        }

                    }

                    if (state)
                    {
                        Print("Nice Thanks for clicking light!");
                    }

                    if (ImGui::Button("Random##dark"))
                    {
                        if (state_dark)
                        {
                            state_dark = false;
                        }
                        else
                        {
                            state_dark = true;
                        }
                    }

                    if (state_dark)
                    {
                        Print("Nice Thanks for clicking dark!");
                    }

                    ImGui::Checkbox("CheckBox", &checkbox);

                    checkbox ? Print("True") : Print("false");


                    //ImGui auto assumes next line
                    ImGui::RadioButton("Button a", &radiobutton, 0); ImGui::SameLine();
                    ImGui::RadioButton("Button b", &radiobutton, 1); ImGui::SameLine();
                    ImGui::RadioButton("Button c", &radiobutton, 2); ImGui::SameLine();

                    switch (radiobutton)
                    {

                    case 0:
                        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Pressed a");
                        break;
                    case 1:
                        ImGui::TextColored(ImVec4(0, 1, 0, 1), "Pressed b");
                        break;
                    case 2:
                        ImGui::TextColored(ImVec4(0, 0, 1, 1), "Pressed c");
                        break;

                    default:
                        break;
                    }
                    //HyperLinks
                        //!)TextLinkUrl
                    ImGui::TextLinkOpenURL("Github SanthoshLk", "https://github.com/Santhoshlk");

                    if(ImGui::TextLink("Print Label"))
                    {
                        Print("Label");
                    }


                    // style buttons
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 0, 0.75));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 0, 0, 0.5));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1,0,0,1));

                    ImGui::Button("Click!");

                    ImGui::PopStyleColor(3);

                    // arrow button with hold to repeat
                    ImGui::PushItemFlag(ImGuiItemFlags_ButtonRepeat,true);
                    ImGui::Text("Hold to Repeat");
                    ImGui::SameLine();

                    if (ImGui::ArrowButton("##left",ImGuiDir_Left))
                    {
                        counter--;
                    }
                    ImGui::SameLine();
                    if (ImGui::ArrowButton("##Right",ImGuiDir_Right))
                    {
                        counter++;
                    }
                    ImGui::PopItemFlag();
                    ImGui::SameLine();
                    ImGui::Text("Counter :%d", counter);

                    // sliders
                    // int float double drag
                    static int count = 2;
                    static float speed = 0.5f;

                    ImGui::SliderInt("Int Slider",&count,0,10);
                    ImGui::SliderFloat("Slider Float", &speed, 0.f, 13.5f, "%.2f");

                    // any static ones
                    static double inc = 5;
                    static double max = 13.5;
                    static double min = 0;
                    ImGui::SliderScalar("Slider Double",ImGuiDataType_Double,&inc,&min,&max,"%.2f");



                    //draggers
                    static float dragger_float = 0.5f;
                    static int dragger_int = 4;
                    static float float2[] = {0.f,1.f};
                    static ImU64 Sized = 2;


                    // this is unbounded
                    ImGui::DragFloat("Drag Float", &dragger_float);

                    ImGui::DragInt("Drag Int",&dragger_int,2,0,10,"%d",ImGuiSliderFlags_AlwaysClamp);

                    ImGui::DragFloat2("Drag float 2 ",float2,0.5f);
                    
                    // any type
                    ImGui::DragScalar("Drag Scalar",ImGuiDataType_U64,&Sized);


                    // Tooltip
                    ImGui::Button("Save Data");

                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                        ImGui::SetTooltip("Saves the data to nvme!");
                    
                    //Rich Tooltip
                    static bool tooltip_check = false;
                    ImGui::Checkbox("Tooltip",&tooltip_check);

                    if (ImGui::BeginItemTooltip())
                    {
                        ImGui::Text("Tooltip check working");
                        ImGui::Separator();
                        ImGui::TextColored(ImVec4(0.f,1.f,0.f,1.f),"GPU OK");
                        ImGui::EndTooltip();
                    }

                    // 
                    ImGui::TextDisabled("Text Tooltip");
                    if (ImGui::BeginItemTooltip())
                    {
                        // this allows 40 character per line on a box
                        ImGui::PushTextWrapPos(ImGui::GetFontSize()*40.f);
                        ImGui::TextUnformatted("Long explanation that wraps instead of running off screen. "
                            "Adding a lot more text here so it goes way past thirty five "
                            "characters per line and you can actually see the difference."
                            );
                        ImGui::PopTextWrapPos();
                        ImGui::EndTooltip();
                    }

                    //Inputs
               // here i am doing a fixed size so no need to do dynamic stack allocation
                    static char name[64] = "Umbra";
                    ImGui::InputText("name", name, sizeof(char)*64);

                    ImGui::Text(name);

                   // default hint
                    static char search[64] = "";    
                    ImGui::InputTextWithHint("##Search","Search from the assets u know",search,sizeof(char)*64);

                    // multiline text
                    static char multiline[128] = "";
                    ImGui::InputTextMultiline("##multiline",multiline,sizeof(char)*128,ImVec2(100.f,100.f));

                    // input int,float,scalar
                    static int input = 0;
                    ImGui::InputInt("Input Int",&input,2);

                    static float input_float = 0.f;
                    ImGui::InputFloat("Input Float", &input_float, 2.5f,0,"%.2f");

                    // color picker and editor

                    static float color_rgb[3] = { 0.f,1.f,0.f };
                    ImGui::ColorEdit3("RGB Color",color_rgb,ImGuiColorEditFlags_DisplayRGB);

                    static float color_rgba[4] = { 0.f,1.f,0.f,1.f };
                    ImGui::ColorEdit4("RGBA Color", color_rgba, ImGuiColorEditFlags_AlphaBar);

                    ImGui::End();
                }

                // add combo and list when used
