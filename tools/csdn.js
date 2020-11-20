// ==UserScript==
// @name         csdn hide adv and show
// @namespace    http://cswhello.com/
// @version      0.1
// @description  this is a test
// @author       cswhello
// @match        https://blog.csdn.net/*
// @match        https://bbs.csdn.net/topics/*
// @require      https://code.jquery.com/jquery-2.1.4.min.js
// @run-at       document-end
// @grant        none
// ==/UserScript==

(function() {
    'use strict';

    var currentURL = window.location.href;
    var bbs = /bbs\.csdn\.net/;
    var blog = /blog\.csdn\.net/;


    if(blog.test(currentURL)){
        console.log("blog");
        $(".csdn-tracking-statistics.mb8.box-shadow").remove();
        $(".recommend-item-box.recommend-ad-box").remove();
        $("#dmp_ad_58").remove();

        $(".pulllog-box").hide();
        $("#adContent").hide();


        //$(".hide-article-box").remove();

        $("#_360_interactive").hide();
        $(".hide-article-box").hide();
        $("#article_content").height("100%");
        $($("#asideFooter").children("div").get(0)).hide();

        csdn.copyright.init("", "", "");
    }else if(bbs.test(currentURL)){
        console.log("blog");
        $("#bbs_detail_wrap").css("max-height","");
        $("#bbs_detail_wrap").css("overflow","");
        $(".mediav_ad").remove();
        $("[class='bbs_feed bbs_feed_ad_box']").remove();

        $(".pulllog-box").hide();
        $("#adContent").hide();


        $(".hide_topic_box").remove();
    }else{
        console.log("unknown");
    }


    // 域名规则列表
    const rules = {
        plus: {
            hook_eventNames: "contextmenu|select|selectstart|copy|cut|dragstart",
            unhook_eventNames: "mousedown|mouseup|keydown|keyup",
            dom0: true,
            hook_addEventListener: true,
            hook_preventDefault: true,
            add_css: true
        }
    };

    const returnTrue = e => true;
    // 获取目标域名应该使用的规则
    const getRule = (host) => rules[host] || rules.plus;
    const dontHook = e => !!e.closest('form');
    // 储存被 Hook 的函数
    const EventTarget_addEventListener = EventTarget.prototype.addEventListener;
    const document_addEventListener = document.addEventListener;
    const Event_preventDefault = Event.prototype.preventDefault;
    // 要处理的 event 列表
    let hook_eventNames, unhook_eventNames, eventNames;

    // Hook addEventListener proc
    function addEventListener(type, func, useCapture) {
        const _addEventListener = this === document ? document_addEventListener : EventTarget_addEventListener;
        const a = hook_eventNames.includes(type) ? [type, returnTrue, useCapture] : arguments;
        _addEventListener.apply(this, a);
    }

    // 清理或还原DOM节点的onxxx属性
    function clearLoop() {
        let type, prop,
            c = [document,document.body, ...document.getElementsByTagName('div')],
            // https://life.tw/?app=view&no=746862
            e = document.querySelector('iframe[src="about:blank"]');
        if (e && e.clientWidth>99 && e.clientHeight>11) {
            e = e.contentWindow.document;
            c.push(e, e.body);
        }

        for (e of c) {
            if (!e) continue;
            e = e.wrappedJSObject || e;
            for (type of eventNames) {
                prop = 'on' + type;
                e[prop] = null;
            }
        }
    }

    function init() {
        // 获取当前域名的规则
        let rule = getRule(location.host);
        // 设置 event 列表
        hook_eventNames = rule.hook_eventNames.split("|");
        // Allowed to return value
        unhook_eventNames = rule.unhook_eventNames.split("|");
        eventNames = hook_eventNames.concat(unhook_eventNames);

        if (rule.dom0) {
            setInterval(clearLoop, 9e3);
            setTimeout(clearLoop, 1e3);
            window.addEventListener('load', clearLoop, true);
        }

        if (rule.hook_addEventListener) {
            EventTarget.prototype.addEventListener = addEventListener;
            document.addEventListener = addEventListener;
        }

        if (rule.hook_preventDefault) {
            Event.prototype.preventDefault = function () {
                if (dontHook(this.target) || !eventNames.includes(this.type)) {
                    Event_preventDefault.apply(this, arguments);
                }
            };
        }

        if (rule.add_css) GM_addStyle(
            `html, * {
-webkit-user-select:text !important;
-moz-user-select:text !important;
user-select:text !important;
}
::-moz-selection {color:#111 !important; background:#05D3F9 !important;}
::selection {color:#111 !important; background:#05D3F9 !important;}`
        );
    }

    init();







})();


