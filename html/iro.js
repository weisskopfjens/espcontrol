  /*!
 * iro.js v3.5.3
 * 2016-2018 James Daniel
 * Released under the MIT License
 * github.com/jaames/iro.js
 */

!function(t,e){"object"==typeof exports&&"undefined"!=typeof module?module.exports=e():"function"==typeof define&&define.amd?define(e):t.iro=e()}(this,function(){"use strict";var t=function(t,e){var r=t.g({class:"iro__marker"});r.circle(0,0,e.r,{class:"iro__marker__outer",fill:"none",strokeWidth:5,stroke:"#000"}),r.circle(0,0,e.r,{class:"iro__marker__inner",fill:"none",strokeWidth:2,stroke:"#fff"}),this.g=r};t.prototype.move=function(t,e){this.g.setTransform("translate",[t,e])};var e=Math.PI,r=Math.sqrt,s=Math.abs,i=Math.round,n=function(e,r){this._opts=r,this.type="wheel";var s=r.cY,i=r.cX,n=r.r,o=r.border,a=e.g({class:"iro__wheel"});a.circle(i,s,n+o.w/2,{class:"iro__wheel__border",fill:"#fff",stroke:o.color,strokeWidth:o.w});for(var h=a.g({class:"iro__wheel__hue",strokeWidth:n,fill:"none"}),c=0;c<360;c++)h.arc(i,s,n/2,c,c+1.5,{stroke:"hsl("+(r.anticlockwise?360-c:c)+",100%,50%)"});a.circle(i,s,n,{class:"iro__wheel__saturation"}).setGradient("fill",e.gradient("radial",{0:{color:"#fff"},100:{color:"#fff",opacity:0}})),this._lightness=a.circle(i,s,n,{class:"iro__wheel__lightness",opacity:0}),this.marker=new t(a,r.marker)};n.prototype.update=function(t,r){var s=this._opts,i=t.hsv;if(r.v&&s.lightness&&this._lightness.setAttrs({opacity:(1-i.v/100).toFixed(2)}),r.h||r.s){var n=(s.anticlockwise?360-i.h:i.h)*(e/180),o=i.s/100*s.rMax;this.marker.move(s.cX+o*Math.cos(n),s.cY+o*Math.sin(n))}},n.prototype.input=function(t,s){var n=this._opts,o=n.rMax,a=n.cX-t,h=n.cY-s,c=Math.atan2(h,a),l=i(c*(180/e))+180,u=Math.min(r(a*a+h*h),o);return{h:l=n.anticlockwise?360-l:l,s:i(100/o*u)}},n.prototype.checkHit=function(t,e){var i=this._opts,n=s(t-i.cX),o=s(e-i.cY);return r(n*n+o*o)<i.r};var o=Math.round,a=Math.floor;function h(t,e){var r=t.match(/(\S+)\((\d+)(%?)(?:\D+?)(\d+)(%?)(?:\D+?)(\d+)(%?)(?:\D+?)?([0-9\.]+?)?\)/i),s=parseInt(r[2]),i=parseInt(r[4]),n=parseInt(r[6]);return[r[1],"%"==r[3]?s/100*e[0]:s,"%"==r[5]?i/100*e[1]:i,"%"==r[7]?n/100*e[2]:n,parseFloat(r[8])||void 0]}function c(t){return t instanceof p?t:new p(t)}function l(t,e,r){return t<=e?e:t>=r?r:t}function u(t,e){var r={};for(var s in t)r[s]=e[s]!=t[s];return r}var p=function(t){this._onChange=!1,this._value={h:void 0,s:void 0,v:void 0},t&&this.set(t)},f={hsv:{configurable:!0},rgb:{configurable:!0},hsl:{configurable:!0},rgbString:{configurable:!0},hexString:{configurable:!0},hslString:{configurable:!0}};p.mix=function(t,e,r){var s=c(t).rgb,i=c(e).rgb;return r=l(r/100||.5,0,1),new p({r:a(s.r+(i.r-s.r)*r),g:a(s.g+(i.g-s.g)*r),b:a(s.b+(i.b-s.b)*r)})},p.lighten=function(t,e){var r=c(t),s=r.hsv;return s.v=l(s.v+e,0,100),r.hsv=s,r},p.darken=function(t,e){var r=c(t),s=r.hsv;return s.v=l(s.v-e,0,100),r.hsv=s,r},p.hsv2Rgb=function(t){var e,r,s,i,n,h,c,l,u=t.h/360,p=t.s/100,f=t.v/100;switch(h=f*(1-p),c=f*(1-(n=6*u-(i=a(6*u)))*p),l=f*(1-(1-n)*p),i%6){case 0:e=f,r=l,s=h;break;case 1:e=c,r=f,s=h;break;case 2:e=h,r=f,s=l;break;case 3:e=h,r=c,s=f;break;case 4:e=l,r=h,s=f;break;case 5:e=f,r=h,s=c}return{r:o(255*e),g:o(255*r),b:o(255*s)}},p.rgb2Hsv=function(t){var e,r=t.r/255,s=t.g/255,i=t.b/255,n=Math.max(r,s,i),o=Math.min(r,s,i),a=n-o;switch(n){case o:e=0;break;case r:e=(s-i)/a+(s<i?6:0);break;case s:e=(i-r)/a+2;break;case i:e=(r-s)/a+4}return{h:360*(e/=6),s:0==n?0:a/n*100,v:100*n}},p.hsv2Hsl=function(t){var e=t.s/100,r=t.v/100,s=.5*r*(2-e);return e=r*e/(1-Math.abs(2*s-1)),{h:t.h,s:100*e||0,l:100*s}},p.hsl2Hsv=function(t){var e=t.s/100,r=t.l/100;return e*=(r*=2)<=1?r:2-r,{h:t.h,s:2*e/(r+e)*100,v:(r+e)/2*100}},p.hsl2Str=function(t){return"hsl"+(t.a?"a":"")+"("+t.h+", "+t.s+"%, "+t.l+"%"+(t.a?", "+t.a:"")+")"},p.rgb2Str=function(t){return"rgb"+(t.a?"a":"")+"("+t.r+", "+t.g+", "+t.b+(t.a?", "+t.a:"")+")"},p.rgb2Hex=function(t){var e="#";return e+=t.r.toString(16).padStart(2,"0"),e+=t.g.toString(16).padStart(2,"0"),e+=t.b.toString(16).padStart(2,"0")},p.parseHexStr=function(t){t=t.replace("#","");var e=parseInt("0x"+t),r=3==t.length,s=r?15:255,i=r?4:8,n=r?17:1;return{r:(e>>2*i&s)*n,g:(e>>i&s)*n,b:(e&s)*n}},p.parseHslStr=function(t){var e=h(t,[360,100,100]);return{h:e[2],s:e[3],l:e[4]}},p.parseRgbStr=function(t){var e=h(t,[255,255,255]);return{r:e[1],g:e[2],b:e[3]}},f.hsv.get=function(){var t=this._value;return{h:t.h,s:t.s,v:t.v}},f.hsv.set=function(t){if(this._onChange){var e=this._value;for(var r in e)t.hasOwnProperty(r)||(t[r]=e[r]);var s=u(e,t);this._value=t,(s.h||s.s||s.v)&&this._onChange(this,s)}else this._value=t},f.rgb.get=function(){var t=p.hsv2Rgb(this._value);return{r:o(t.r),g:o(t.g),b:o(t.b)}},f.rgb.set=function(t){this.hsv=p.rgb2Hsv(t)},f.hsl.get=function(){var t=p.hsv2Hsl(this._value);return{h:o(t.h),s:o(t.s),l:o(t.l)}},f.hsl.set=function(t){this.hsv=p.hsl2Hsv(t)},f.rgbString.get=function(){return p.rgb2Str(this.rgb)},f.rgbString.set=function(t){this.rgb=p.parseRgbStr(t)},f.hexString.get=function(){return p.rgb2Hex(this.rgb)},f.hexString.set=function(t){this.rgb=p.parseHexStr(t)},f.hslString.get=function(){return p.hsl2Str(this.hsl)},f.hslString.set=function(t){this.hsl=p.parseHslStr(t)},p.prototype.set=function(t){"object"==typeof t?t instanceof p?this.hsv=p.hsv:"r"in t?this.rgb=t:"v"in t?this.hsv=t:"l"in t&&(this.hsl=t):"string"==typeof t&&(/^rgb/.test(t)?this.rgbString=t:/^hsl/.test(t)?this.hslString=t:/^#[0-9A-Fa-f]/.test(t)&&(this.hexString=t))},p.prototype.setChannel=function(t,e,r){var s=this[t];s[e]=r,this[t]=s},p.prototype.clone=function(){return new p(this)},p.prototype.compare=function(t,e){return u(this[e=e||"hsv"],c(t)[e])},p.prototype.mix=function(t,e){this.hsv=mix(this,t,e).hsv},p.prototype.lighten=function(t){lighten(this,t)},p.prototype.darken=function(t){darken(this,t)},Object.defineProperties(p.prototype,f);var v=function(e,r){var s=r.r,i=r.w,n=r.h,o=r.x,a=r.y,h=r.border.w;r.range={min:o+s,max:o+i-s,w:i-2*s},r.sliderType=r.sliderType||"v",this.type="slider",this._opts=r;var c=s+h/2,l=e.g({class:"iro__slider"}),u=l.insert("rect",{class:"iro__slider__value",rx:c,ry:c,x:o-h/2,y:a-h/2,width:i+h,height:n+h,strokeWidth:h,stroke:r.border.color});u.setGradient("fill",e.gradient("linear",{0:{color:"#000"},100:{color:"#fff"}})),this._gradient=u.gradient,this.marker=new t(l,r.marker)};v.prototype.update=function(t,e){var r=this._opts,s=r.range,i=t.hsv,n=p.hsv2Hsl({h:i.h,s:i.s,v:100});if("v"==r.sliderType&&((e.h||e.s)&&this._gradient.stops[1].setAttrs({stopColor:"hsl("+n.h+","+n.s+"%,"+n.l+"%)"}),e.v)){var o=i.v/100;this.marker.move(s.min+o*s.w,r.y+r.h/2)}},v.prototype.input=function(t,e){var r=this._opts.range,s=Math.max(Math.min(t,r.max),r.min)-r.min;return{v:Math.round(100/r.w*s)}},v.prototype.checkHit=function(t,e){var r=this._opts;return t>r.x&&t<r.x+r.w&&e>r.y&&e<r.y+r.h};var g=0,d={class:"class",stroke:"stroke",strokeWidth:"stroke-width",fill:"fill",opacity:"opacity",offset:"offset",stopColor:"stop-color",stopOpacity:"stop-opacity"},m={translate:"setTranslate",scale:"setScale",rotate:"setRotate"},_=window.navigator.userAgent.toLowerCase(),y=/msie|trident|edge/.test(_),b=/^((?!chrome|android).)*safari/i.test(_),w=function(t,e,r,s){var i=document.createElementNS("http://www.w3.org/2000/svg",r);this.el=i,this.setAttrs(s),(e.el||e).appendChild(i),this._root=t,this._svgTransforms={},this._transformList=!!i.transform&&i.transform.baseVal};w.prototype.insert=function(t,e){return new w(this._root,this,t,e)},w.prototype.g=function(t){return this.insert("g",t)},w.prototype.arc=function(t,e,r,s,i,n){var o=i-s<=180?0:1;s*=Math.PI/180,i*=Math.PI/180;var a=t+r*Math.cos(i),h=e+r*Math.sin(i),c=t+r*Math.cos(s),l=e+r*Math.sin(s);return(n=n||{}).d=["M",a,h,"A",r,r,0,o,0,c,l].join(" "),this.insert("path",n)},w.prototype.circle=function(t,e,r,s){return(s=s||{}).cx=t,s.cy=e,s.r=r,this.insert("circle",s)},w.prototype.setTransform=function(t,e){if(y)this.setAttrs({transform:t+"("+e.join(", ")+")"});else{var r,s=this._svgTransforms;s[t]?r=s[t]:(r=this._root.el.createSVGTransform(),s[t]=r,this._transformList.appendItem(r)),r[t in m?m[t]:t].apply(r,e)}},w.prototype.setAttrs=function(t){for(var e in t){var r=e in d?d[e]:e;this.el.setAttribute(r,t[e])}},w.prototype.setGradient=function(t,e){var r={};r[t]=e.getUrl(),e._refs[t]=this,this.gradient=e,this.setAttrs(r)};var k=function(t,e,r){var s=[],i=t._defs.insert(e+"Gradient",{id:"iroGradient"+g++});for(var n in r){var o=r[n];s.push(i.insert("stop",{offset:n+"%",stopColor:o.color,stopOpacity:void 0===o.opacity?1:o.opacity}))}this.el=i.el,this.stops=s,this._refs={}};k.prototype.getUrl=function(t){return"url("+(b?t||location.protocol+"//"+location.host+location.pathname:"")+"#"+this.el.id+")"};var S=function(t){function e(e,r,s,i){t.call(this,null,e,"svg",{width:r,height:s,style:"display:"+(i||"block")+";touch-action:none"}),this._root=this,this._defs=this.insert("defs"),this._gradients=[]}return t&&(e.__proto__=t),e.prototype=Object.create(t&&t.prototype),e.prototype.constructor=e,e.prototype.gradient=function(t,e){var r=new k(this,t,e);return this._gradients.push(r),r},e.prototype.updateUrls=function(t){if(b)for(var e=this._gradients,r=0;r<e.length;r++)for(var s in e[r]._refs){var i={};i[s]=e[r].getUrl(t),e[r]._refs[s].setAttrs(i)}},e}(w),x=function(){var t=document.createElement("style");document.head.appendChild(t),t.appendChild(document.createTextNode("")),this.style=t;var e=t.sheet;this.sheet=e,this.rules=e.rules||e.cssRules,this.map={}},M={enabled:{configurable:!0},cssText:{configurable:!0},css:{configurable:!0}};M.enabled.get=function(){return!this.sheet.disabled},M.enabled.set=function(t){this.sheet.disabled=!t},M.cssText.get=function(){var t=this.map,e=[];for(var r in t)e.push(r.replace(/,\W/g,",\n")+" {\n\t"+t[r].cssText.replace(/;\W/g,";\n\t")+"\n}");return e.join("\n")},M.css.get=function(){var t=this.map,e={};for(var r in t){var s=t[r];e[r]={};for(var i=0;i<s.length;i++){var n=s[i];e[r][n]=s.getPropertyValue(n)}}return e},x.prototype.setRule=function(t,e,r){var s=this.sheet,i=s.rules||s.cssRules,n=this.map;if(e=e.replace(/([A-Z])/g,function(t){return"-"+t.toLowerCase()}),n.hasOwnProperty(t))n[t].setProperty(e,r);else{var o=i.length,a=e+": "+r;try{s.insertRule(t+" {"+a+";}",o)}catch(e){s.addRule(t,a,o)}finally{i=s.rules||s.cssRules,n[t]=i[o].style}}},Object.defineProperties(x.prototype,M);var C="readystatechange",T="complete";function H(t,e,r,s){void 0===s&&(s={});for(var i=0;i<e.length;i++)t.addEventListener(e[i],r,s)}function A(t,e,r){for(var s=0;s<e.length;s++)t.removeEventListener(e[s],r)}var R=function(t,e){var r,s=this;e=e||{},this._events={},this._mouseTarget=!1,this._colorChangeActive=!1,this.css=e.css||e.styles||void 0,r=function(){s._mount(t,e)},document.readyState==T?r():H(document,[C],function t(e){document.readyState==T&&(r(),A(document,[C],t))})};return R.prototype._mount=function(t,e){var r=this;t="string"==typeof t?document.querySelector(t):t;var s=parseInt(getComputedStyle(t).width),i=parseInt(getComputedStyle(t).height);i<s&&0!=i?s=i:0!=s&&(i=s);var o=e.width||s||320,a=e.height||i||320,h=e.padding+2||6,c=e.borderWidth||0,l=e.markerRadius||8,u=e.sliderMargin||24,f=e.sliderHeight||2*l+2*h+2*c,g=Math.min(a-f-u,o),d=g/2-c,m=(o-g)/2,_={r:l},y={w:c,color:e.borderColor||"#fff"};this.el=t,this.svg=new S(t,o,a,e.display),this.ui=[new n(this.svg,{cX:m+g/2,cY:g/2,r:d,rMax:d-(l+h),marker:_,border:y,lightness:void 0==e.wheelLightness||e.wheelLightness,anticlockwise:e.anticlockwise}),new v(this.svg,{sliderType:"v",x:m+c,y:g+u,w:g-2*c,h:f-2*c,r:f/2-c,marker:_,border:y})],this.stylesheet=new x,this.color=new p,this.color._onChange=this._update.bind(this),this.color.set(e.color||e.defaultValue||"#fff"),this.on("history:stateChange",function(t){r.svg.updateUrls(t)}),H(this.svg.el,["mousedown","touchstart"],this,{passive:!1}),this.emit("mount",this)},R.prototype._update=function(t,e){for(var r=t.rgbString,s=this.css,i=0;i<this.ui.length;i++)this.ui[i].update(t,e);for(var n in s){var o=s[n];for(var a in o)this.stylesheet.setRule(n,a,r)}this._colorChangeActive||(this._colorChangeActive=!0,this.emit("color:change",t,e),this._colorChangeActive=!1)},R.prototype.on=function(t,e){var r=this._events;(r[t]||(r[t]=[])).push(e)},R.prototype.off=function(t,e){var r=this._events[t];r&&r.splice(r.indexOf(e),1)},R.prototype.emit=function(t){for(var e=[],r=arguments.length-1;r-- >0;)e[r]=arguments[r+1];for(var s=this._events,i=(s[t]||[]).concat(s["*"]||[]),n=0;n<i.length;n++)i[n].apply(null,e)},R.prototype.handleEvent=function(t){var e=t.touches?t.changedTouches[0]:t,r=this.svg.el.getBoundingClientRect(),s=e.clientX-r.left,i=e.clientY-r.top;switch(t.type){case"mousedown":case"touchstart":for(var n=0;n<this.ui.length;n++){var o=this.ui[n];o.checkHit(s,i)&&(this._mouseTarget=o,H(document,["mousemove","touchmove","mouseup","touchend"],this,{passive:!1}),this.emit("input:start",this.color),this.color.hsv=this._mouseTarget.input(s,i))}break;case"mousemove":case"touchmove":this.color.hsv=this._mouseTarget.input(s,i);break;case"mouseup":case"touchend":this._mouseTarget=!1,this.emit("input:end",this.color),A(document,["mousemove","touchmove","mouseup","touchend"],this)}this._mouseTarget&&t.preventDefault()},{Color:p,ColorPicker:R,Stylesheet:x,version:"3.5.3"}});
//# sourceMappingURL=iro.min.js.map