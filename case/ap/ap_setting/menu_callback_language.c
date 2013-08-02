/*
 *******************************************************************************
 *                              USDK
 *                             Module: SETTING AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : menu_callback_language.C
 * History:
 *      <johnsen>    <2011-9-25 19:25>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, 本应用的头文件 */
#include "ap_setting.h"
#include "menu_callback_language.h"


const uint16 language_text_table[LANGUAGE_TOTAL] =
{ 
    S_LANGUAGE_CZECH, S_LANGUAGE_GREEK, S_LANGUAGE_ENGLISH, S_LANGUAGE_DENISH, S_LANGUAGE_GERMANY,
    S_LANGUAGE_SPANISH, S_LANGUAGE_FRENCH, S_LANGUAGE_ITALIAN, S_LANGUAGE_HUNGARIAN, S_LANGUAGE_DUTCH,
    S_LANGUAGE_NORWEGIAN, S_LANGUAGE_POLAND, S_LANGUAGE_PORTUGUESE_EUROPEAN, S_LANGUAGE_PORTUGUESE_BRAZILIAN,
    S_LANGUAGE_RUSSIAN, S_LANGUAGE_SLOVAK, S_LANGUAGE_FINNISH, S_LANGUAGE_SWEDISH, S_LANGUAGE_TURKEY, 
    S_LANGUAGE_SIMPLIFIED_CHINESE,S_LANGUAGE_TRAD_CHINESE, S_LANGUAGE_JAPANESE, S_LANGUAGE_KOREAN,
    S_LANGUAGE_HEBREW,S_LANGUAGE_ARABIC,S_LANGUAGE_THAI,S_LANGUAGE_ROMANIAN,S_LANGUAGE_INDONESIAN
}; /*此表必须与显示驱动安排完全一致，由显示驱动决定，并且必须连续且完整。*/


//一级菜单-language/语言
//language/语言
//             |-捷克语
//             |-希腊语
//             |-英语
//             |-丹麦语
//             |-德语
//             |-西班牙语
//             |-法语
//             |-意大利语
//             |-匈牙利语
//             |-荷兰语
//             |-挪威语
//             |-波兰语
//             |-葡萄牙语-欧洲
//             |-葡萄牙语-巴西
//             |-俄语
//             |-斯洛伐克语
//             |-芬兰语
//             |-瑞典语
//             |-土耳其语
//             |-简体中文
//             |-繁体中文
//             |-日语
//             |-韩文
//             |-希伯来语
//             |-阿拉伯语
//             |-泰文
//             |-罗马语
//             |-印尼语
//==============下面几个暂不支持=====================
//             |-克罗地亚语
//             |-斯洛文尼亚语



////////////////////

//从语言列表菜单退出时回调函数
void set_language_id_back2parent(void)
{
    ui_set_lang_option(LANG_LIST_MENU_EXIT);
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于取得当前语言选择
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 返回语言选项string ID
 * \retval       ：进入下一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e get_language_id_callback(void * title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title_language;
    menu_title_language = (menu_title_action_t*) title_action;
    active_id = language_text_table[g_setting_vars.g_comval.language_id];
    menu_title_language->menu_entry = -1; //不更新入口
    menu_title_language->str_id = active_id; //指定active项
    menu_title_language->ret_str = NULL;
    menu_title_language->source_id = -1;
    
    //进入语言列表菜单，菜单项字符串特殊处理
    ui_set_lang_option(LANG_LIST_MENU_ENTER);
    //从语言列表菜单退出时调用 set_language_id_back2parent
    set_language_id_argv = 1;
    gui_menulist_set_back2parent(set_language_id_back2parent, &set_language_id_argv);
    
    return (app_result_e) RESULT_MENU_SON; //进入下一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为简体中文
 *      重点说明m
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_simplified_chinese_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_SIMPLIFIED_CHINESE;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为英文
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_english_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_ENGLISH;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为繁体中文
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_trad_chinese_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_TRAD_CHINESE;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为日文
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_japanese_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_JAPANESE;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为韩文
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_korean_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_KOREAN;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为法文
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_french_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_FRENCH;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为德文
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_germany_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_GERMANY;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为意大利文
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_italian_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_ITALIAN;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为荷兰语
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_dutch_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_DUTCH;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}


/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为西班牙语
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_spanish_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_SPANISH;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为瑞典语
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_swedish_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_SWEDISH;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为捷克语
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_czech_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_CZECH;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为丹麦语
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_denish_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_DENISH;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于用于将语言设置为波兰语
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_poland_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_POLAND;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为俄语
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_russian_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_RUSSIAN;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为土耳其语
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_turkey_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_TURKEY;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为希伯来语
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_hebrew_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_HEBREW;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为泰文
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_thai_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_THAI;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为匈牙利语
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_hungarian_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_HUNGARIAN;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为罗马语
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_romanian_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_ROMANIAN;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为阿拉伯语
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_arabic_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_ARABIC;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为克罗地亚语
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_croatian_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_CROATIAN;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为斯洛文尼亚语
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_slovenian_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_SLOVENIAN;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}


/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为希腊语
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_greek_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_GREEK;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为挪威语
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_norwegian_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_NORWEGIAN;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为葡萄牙语-欧洲
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_portuguese_european_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_PORTUGUESE_EUROPEAN;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为葡萄牙语-巴西
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_portuguese_brazilian_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_PORTUGUESE_BRAZILIAN;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为斯洛伐克语
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_slovak_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_SLOVAK;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为芬兰语
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_finnish_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_FINNISH;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}


/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于将语言设置为印尼语
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_indonesian_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_INDONESIAN;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

