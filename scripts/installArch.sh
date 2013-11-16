#!/bin/bash

# (C) 2013 by NIPE-SYSTEMS.de
# This script will install an Linux Arch with all dependencies for Roboter CAN

# LOCALE GENERATION
echo "# Configuration file for locale-gen
#
# lists of locales that are to be generated by the locale-gen command.
#
# Each line is of the form:
#
#     <locale> <charset>
#
#  where <locale> is one of the locales given in /usr/share/i18n/locales
#  and <charset> is one of the character sets listed in /usr/share/i18n/charmaps
#
#  Examples:
#  en_US ISO-8859-1
#  en_US.UTF-8 UTF-8
#  de_DE ISO-8859-1
#  de_DE@euro ISO-8859-15
#
#  The locale-gen command will generate all the locales,
#  placing them in /usr/lib/locale.
#
#  A list of supported locales is included in this file.
#  Uncomment the ones you need.
#
#aa_DJ.UTF-8 UTF-8  
#aa_DJ ISO-8859-1  
#aa_ER UTF-8  
#aa_ER@saaho UTF-8  
#aa_ET UTF-8  
#af_ZA.UTF-8 UTF-8  
#af_ZA ISO-8859-1  
#am_ET UTF-8  
#an_ES.UTF-8 UTF-8  
#an_ES ISO-8859-15  
#ar_AE.UTF-8 UTF-8  
#ar_AE ISO-8859-6  
#ar_BH.UTF-8 UTF-8  
#ar_BH ISO-8859-6  
#ar_DZ.UTF-8 UTF-8  
#ar_DZ ISO-8859-6  
#ar_EG.UTF-8 UTF-8  
#ar_EG ISO-8859-6  
#ar_IN UTF-8  
#ar_IQ.UTF-8 UTF-8  
#ar_IQ ISO-8859-6  
#ar_JO.UTF-8 UTF-8  
#ar_JO ISO-8859-6  
#ar_KW.UTF-8 UTF-8  
#ar_KW ISO-8859-6  
#ar_LB.UTF-8 UTF-8  
#ar_LB ISO-8859-6  
#ar_LY.UTF-8 UTF-8  
#ar_LY ISO-8859-6  
#ar_MA.UTF-8 UTF-8  
#ar_MA ISO-8859-6  
#ar_OM.UTF-8 UTF-8  
#ar_OM ISO-8859-6  
#ar_QA.UTF-8 UTF-8  
#ar_QA ISO-8859-6  
#ar_SA.UTF-8 UTF-8  
#ar_SA ISO-8859-6  
#ar_SD.UTF-8 UTF-8  
#ar_SD ISO-8859-6  
#ar_SY.UTF-8 UTF-8  
#ar_SY ISO-8859-6  
#ar_TN.UTF-8 UTF-8  
#ar_TN ISO-8859-6  
#ar_YE.UTF-8 UTF-8  
#ar_YE ISO-8859-6  
#ayc_PE UTF-8  
#az_AZ UTF-8  
#as_IN UTF-8  
#ast_ES.UTF-8 UTF-8  
#ast_ES ISO-8859-15  
#be_BY.UTF-8 UTF-8  
#be_BY CP1251  
#be_BY@latin UTF-8  
#bem_ZM UTF-8  
#ber_DZ UTF-8  
#ber_MA UTF-8  
#bg_BG.UTF-8 UTF-8  
#bg_BG CP1251  
#bho_IN UTF-8  
#bn_BD UTF-8  
#bn_IN UTF-8  
#bo_CN UTF-8  
#bo_IN UTF-8  
#br_FR.UTF-8 UTF-8  
#br_FR ISO-8859-1  
#br_FR@euro ISO-8859-15  
#brx_IN UTF-8  
#bs_BA.UTF-8 UTF-8  
#bs_BA ISO-8859-2  
#byn_ER UTF-8  
#ca_AD.UTF-8 UTF-8  
#ca_AD ISO-8859-15  
#ca_ES.UTF-8 UTF-8  
#ca_ES ISO-8859-1  
#ca_ES@euro ISO-8859-15  
#ca_FR.UTF-8 UTF-8  
#ca_FR ISO-8859-15  
#ca_IT.UTF-8 UTF-8  
#ca_IT ISO-8859-15  
#crh_UA UTF-8  
#cs_CZ.UTF-8 UTF-8  
#cs_CZ ISO-8859-2  
#csb_PL UTF-8  
#cv_RU UTF-8  
#cy_GB.UTF-8 UTF-8  
#cy_GB ISO-8859-14  
#da_DK.UTF-8 UTF-8  
#da_DK ISO-8859-1  
#de_AT.UTF-8 UTF-8  
#de_AT ISO-8859-1  
#de_AT@euro ISO-8859-15  
#de_BE.UTF-8 UTF-8  
#de_BE ISO-8859-1  
#de_BE@euro ISO-8859-15  
#de_CH.UTF-8 UTF-8  
#de_CH ISO-8859-1  
de_DE.UTF-8 UTF-8  
#de_DE ISO-8859-1  
#de_DE@euro ISO-8859-15  
#de_LU.UTF-8 UTF-8  
#de_LU ISO-8859-1  
#de_LU@euro ISO-8859-15  
#doi_IN UTF-8  
#dv_MV UTF-8  
#dz_BT UTF-8  
#el_GR.UTF-8 UTF-8  
#el_GR ISO-8859-7  
#el_CY.UTF-8 UTF-8  
#el_CY ISO-8859-7  
#en_AG UTF-8  
#en_AU.UTF-8 UTF-8  
#en_AU ISO-8859-1  
#en_BW.UTF-8 UTF-8  
#en_BW ISO-8859-1  
#en_CA.UTF-8 UTF-8  
#en_CA ISO-8859-1  
#en_DK.UTF-8 UTF-8  
#en_DK ISO-8859-1  
#en_GB.UTF-8 UTF-8  
#en_GB ISO-8859-1  
#en_HK.UTF-8 UTF-8  
#en_HK ISO-8859-1  
#en_IE.UTF-8 UTF-8  
#en_IE ISO-8859-1  
#en_IE@euro ISO-8859-15  
#en_IN UTF-8  
#en_NG UTF-8  
#en_NZ.UTF-8 UTF-8  
#en_NZ ISO-8859-1  
#en_PH.UTF-8 UTF-8  
#en_PH ISO-8859-1  
#en_SG.UTF-8 UTF-8  
#en_SG ISO-8859-1  
#en_US.UTF-8 UTF-8  
#en_US ISO-8859-1  
#en_ZA.UTF-8 UTF-8  
#en_ZA ISO-8859-1  
#en_ZM UTF-8  
#en_ZW.UTF-8 UTF-8  
#en_ZW ISO-8859-1  
#es_AR.UTF-8 UTF-8  
#es_AR ISO-8859-1  
#es_BO.UTF-8 UTF-8  
#es_BO ISO-8859-1  
#es_CL.UTF-8 UTF-8  
#es_CL ISO-8859-1  
#es_CO.UTF-8 UTF-8  
#es_CO ISO-8859-1  
#es_CR.UTF-8 UTF-8  
#es_CR ISO-8859-1  
#es_CU UTF-8  
#es_DO.UTF-8 UTF-8  
#es_DO ISO-8859-1  
#es_EC.UTF-8 UTF-8  
#es_EC ISO-8859-1  
#es_ES.UTF-8 UTF-8  
#es_ES ISO-8859-1  
#es_ES@euro ISO-8859-15  
#es_GT.UTF-8 UTF-8  
#es_GT ISO-8859-1  
#es_HN.UTF-8 UTF-8  
#es_HN ISO-8859-1  
#es_MX.UTF-8 UTF-8  
#es_MX ISO-8859-1  
#es_NI.UTF-8 UTF-8  
#es_NI ISO-8859-1  
#es_PA.UTF-8 UTF-8  
#es_PA ISO-8859-1  
#es_PE.UTF-8 UTF-8  
#es_PE ISO-8859-1  
#es_PR.UTF-8 UTF-8  
#es_PR ISO-8859-1  
#es_PY.UTF-8 UTF-8  
#es_PY ISO-8859-1  
#es_SV.UTF-8 UTF-8  
#es_SV ISO-8859-1  
#es_US.UTF-8 UTF-8  
#es_US ISO-8859-1  
#es_UY.UTF-8 UTF-8  
#es_UY ISO-8859-1  
#es_VE.UTF-8 UTF-8  
#es_VE ISO-8859-1  
#et_EE.UTF-8 UTF-8  
#et_EE ISO-8859-1  
#et_EE.ISO-8859-15 ISO-8859-15  
#eu_ES.UTF-8 UTF-8  
#eu_ES ISO-8859-1  
#eu_ES@euro ISO-8859-15  
#fa_IR UTF-8  
#ff_SN UTF-8  
#fi_FI.UTF-8 UTF-8  
#fi_FI ISO-8859-1  
#fi_FI@euro ISO-8859-15  
#fil_PH UTF-8  
#fo_FO.UTF-8 UTF-8  
#fo_FO ISO-8859-1  
#fr_BE.UTF-8 UTF-8  
#fr_BE ISO-8859-1  
#fr_BE@euro ISO-8859-15  
#fr_CA.UTF-8 UTF-8  
#fr_CA ISO-8859-1  
#fr_CH.UTF-8 UTF-8  
#fr_CH ISO-8859-1  
#fr_FR.UTF-8 UTF-8  
#fr_FR ISO-8859-1  
#fr_FR@euro ISO-8859-15  
#fr_LU.UTF-8 UTF-8  
#fr_LU ISO-8859-1  
#fr_LU@euro ISO-8859-15  
#fur_IT UTF-8  
#fy_NL UTF-8  
#fy_DE UTF-8  
#ga_IE.UTF-8 UTF-8  
#ga_IE ISO-8859-1  
#ga_IE@euro ISO-8859-15  
#gd_GB.UTF-8 UTF-8  
#gd_GB ISO-8859-15  
#gez_ER UTF-8  
#gez_ER@abegede UTF-8  
#gez_ET UTF-8  
#gez_ET@abegede UTF-8  
#gl_ES.UTF-8 UTF-8  
#gl_ES ISO-8859-1  
#gl_ES@euro ISO-8859-15  
#gu_IN UTF-8  
#gv_GB.UTF-8 UTF-8  
#gv_GB ISO-8859-1  
#ha_NG UTF-8  
#he_IL.UTF-8 UTF-8  
#he_IL ISO-8859-8  
#hi_IN UTF-8  
#hne_IN UTF-8  
#hr_HR.UTF-8 UTF-8  
#hr_HR ISO-8859-2  
#hsb_DE ISO-8859-2  
#hsb_DE.UTF-8 UTF-8  
#ht_HT UTF-8  
#hu_HU.UTF-8 UTF-8  
#hu_HU ISO-8859-2  
#hy_AM UTF-8  
#hy_AM.ARMSCII-8 ARMSCII-8  
#ia_FR UTF-8  
#id_ID.UTF-8 UTF-8  
#id_ID ISO-8859-1  
#ig_NG UTF-8  
#ik_CA UTF-8  
#is_IS.UTF-8 UTF-8  
#is_IS ISO-8859-1  
#it_CH.UTF-8 UTF-8  
#it_CH ISO-8859-1  
#it_IT.UTF-8 UTF-8  
#it_IT ISO-8859-1  
#it_IT@euro ISO-8859-15  
#iu_CA UTF-8  
#iw_IL.UTF-8 UTF-8  
#iw_IL ISO-8859-8  
#ja_JP.EUC-JP EUC-JP  
#ja_JP.UTF-8 UTF-8  
#ka_GE.UTF-8 UTF-8  
#ka_GE GEORGIAN-PS  
#kk_KZ.UTF-8 UTF-8  
#kk_KZ PT154  
#kl_GL.UTF-8 UTF-8  
#kl_GL ISO-8859-1  
#km_KH UTF-8  
#kn_IN UTF-8  
#ko_KR.EUC-KR EUC-KR  
#ko_KR.UTF-8 UTF-8  
#kok_IN UTF-8  
#ks_IN UTF-8  
#ks_IN@devanagari UTF-8  
#ku_TR.UTF-8 UTF-8  
#ku_TR ISO-8859-9  
#kw_GB.UTF-8 UTF-8  
#kw_GB ISO-8859-1  
#ky_KG UTF-8  
#lb_LU UTF-8  
#lg_UG.UTF-8 UTF-8  
#lg_UG ISO-8859-10  
#li_BE UTF-8  
#li_NL UTF-8  
#lij_IT UTF-8  
#lo_LA UTF-8  
#lt_LT.UTF-8 UTF-8  
#lt_LT ISO-8859-13  
#lv_LV.UTF-8 UTF-8  
#lv_LV ISO-8859-13  
#mag_IN UTF-8  
#mai_IN UTF-8  
#mg_MG.UTF-8 UTF-8  
#mg_MG ISO-8859-15  
#mhr_RU UTF-8  
#mi_NZ.UTF-8 UTF-8  
#mi_NZ ISO-8859-13  
#mk_MK.UTF-8 UTF-8  
#mk_MK ISO-8859-5  
#ml_IN UTF-8  
#mn_MN UTF-8  
#mni_IN UTF-8  
#mr_IN UTF-8  
#ms_MY.UTF-8 UTF-8  
#ms_MY ISO-8859-1  
#mt_MT.UTF-8 UTF-8  
#mt_MT ISO-8859-3  
#my_MM UTF-8  
#nan_TW@latin UTF-8  
#nb_NO.UTF-8 UTF-8  
#nb_NO ISO-8859-1  
#nds_DE UTF-8  
#nds_NL UTF-8  
#ne_NP UTF-8  
#nhn_MX UTF-8  
#niu_NU UTF-8  
#niu_NZ UTF-8  
#nl_AW UTF-8  
#nl_BE.UTF-8 UTF-8  
#nl_BE ISO-8859-1  
#nl_BE@euro ISO-8859-15  
#nl_NL.UTF-8 UTF-8  
#nl_NL ISO-8859-1  
#nl_NL@euro ISO-8859-15  
#nn_NO.UTF-8 UTF-8  
#nn_NO ISO-8859-1  
#nr_ZA UTF-8  
#nso_ZA UTF-8  
#oc_FR.UTF-8 UTF-8  
#oc_FR ISO-8859-1  
#om_ET UTF-8  
#om_KE.UTF-8 UTF-8  
#om_KE ISO-8859-1  
#or_IN UTF-8  
#os_RU UTF-8  
#pa_IN UTF-8  
#pa_PK UTF-8  
#pap_AN UTF-8  
#pl_PL.UTF-8 UTF-8  
#pl_PL ISO-8859-2  
#ps_AF UTF-8  
#pt_BR.UTF-8 UTF-8  
#pt_BR ISO-8859-1  
#pt_PT.UTF-8 UTF-8  
#pt_PT ISO-8859-1  
#pt_PT@euro ISO-8859-15  
#ro_RO.UTF-8 UTF-8  
#ro_RO ISO-8859-2  
#ru_RU.KOI8-R KOI8-R  
#ru_RU.UTF-8 UTF-8  
#ru_RU ISO-8859-5  
#ru_UA.UTF-8 UTF-8  
#ru_UA KOI8-U  
#rw_RW UTF-8  
#sa_IN UTF-8  
#sat_IN UTF-8  
#sc_IT UTF-8  
#sd_IN UTF-8  
#sd_IN@devanagari UTF-8  
#se_NO UTF-8  
#shs_CA UTF-8  
#si_LK UTF-8  
#sid_ET UTF-8  
#sk_SK.UTF-8 UTF-8  
#sk_SK ISO-8859-2  
#sl_SI.UTF-8 UTF-8  
#sl_SI ISO-8859-2  
#so_DJ.UTF-8 UTF-8  
#so_DJ ISO-8859-1  
#so_ET UTF-8  
#so_KE.UTF-8 UTF-8  
#so_KE ISO-8859-1  
#so_SO.UTF-8 UTF-8  
#so_SO ISO-8859-1  
#sq_AL.UTF-8 UTF-8  
#sq_AL ISO-8859-1  
#sq_MK UTF-8  
#sr_ME UTF-8  
#sr_RS UTF-8  
#sr_RS@latin UTF-8  
#ss_ZA UTF-8  
#st_ZA.UTF-8 UTF-8  
#st_ZA ISO-8859-1  
#sv_FI.UTF-8 UTF-8  
#sv_FI ISO-8859-1  
#sv_FI@euro ISO-8859-15  
#sv_SE.UTF-8 UTF-8  
#sv_SE ISO-8859-1  
#sw_KE UTF-8  
#sw_TZ UTF-8  
#szl_PL UTF-8  
#ta_IN UTF-8  
#ta_LK UTF-8  
#te_IN UTF-8  
#tg_TJ.UTF-8 UTF-8  
#tg_TJ KOI8-T  
#th_TH.UTF-8 UTF-8  
#th_TH TIS-620  
#ti_ER UTF-8  
#ti_ET UTF-8  
#tig_ER UTF-8  
#tk_TM UTF-8  
#tl_PH.UTF-8 UTF-8  
#tl_PH ISO-8859-1  
#tn_ZA UTF-8  
#tr_CY.UTF-8 UTF-8  
#tr_CY ISO-8859-9  
#tr_TR.UTF-8 UTF-8  
#tr_TR ISO-8859-9  
#ts_ZA UTF-8  
#tt_RU UTF-8  
#tt_RU@iqtelif UTF-8  
#ug_CN UTF-8  
#uk_UA.UTF-8 UTF-8  
#uk_UA KOI8-U  
#unm_US UTF-8  
#ur_IN UTF-8  
#ur_PK UTF-8  
#uz_UZ ISO-8859-1  
#uz_UZ@cyrillic UTF-8  
#ve_ZA UTF-8  
#vi_VN UTF-8  
#wa_BE ISO-8859-1  
#wa_BE@euro ISO-8859-15  
#wa_BE.UTF-8 UTF-8  
#wae_CH UTF-8  
#wal_ET UTF-8  
#wo_SN UTF-8  
#xh_ZA.UTF-8 UTF-8  
#xh_ZA ISO-8859-1  
#yi_US.UTF-8 UTF-8  
#yi_US CP1255  
#yo_NG UTF-8  
#yue_HK UTF-8  
#zh_CN.GB18030 GB18030  
#zh_CN.GBK GBK  
#zh_CN.UTF-8 UTF-8  
#zh_CN GB2312  
#zh_HK.UTF-8 UTF-8  
#zh_HK BIG5-HKSCS  
#zh_SG.UTF-8 UTF-8  
#zh_SG.GBK GBK  
#zh_SG GB2312  
#zh_TW.EUC-TW EUC-TW  
#zh_TW.UTF-8 UTF-8  
#zh_TW BIG5  
#zu_ZA.UTF-8 UTF-8  
#zu_ZA ISO-8859-1" > /etc/locale.gen

locale-gen

# KEYMAP
localectl set-keymap --no-convert de-latin1

# TIME ZONE
ln -sf /usr/share/zoneinfo/Europe/Berlin /etc/localtime

# FULL SYSTEM UPGRADE
pacman -Syu --noconfirm

# INSTALL WLAN ACCESS POINT
pacman -S wireless_tools dhcp --noconfirm

# UPDATE HOSTNAME
sed -i 's/alarmpi/robotercan/g' /etc/hosts
sed -i 's/alarmpi/robotercan/g' /etc/hostname

# UPDATE /etc/dhcpd.conf
echo '#
# Roboter CAN DHCP Configuration
#
#

ddns-update-style none;

option domain-name "Roboter CAN";

default-lease-time 7200;
max-lease-time 10800;

log-facility local7;

subnet 10.0.0.0 netmask 255.255.255.0 {
  range 10.0.0.2 10.0.0.3;
  option broadcast-address 10.0.0.255;
}

host NIPE {
  hardware ethernet 78:92:9c:00:9a:c6;
  fixed-address 10.0.0.2;
  option host-name "NIPE";
}' > /etc/dhcpd.conf

# UPDATE /root/can-wlan-setup AND MAKE IT EXECUTABLE
echo '#!/bin/bash

echo " > DHCPD STOP"
systemctl stop dhcpd4

echo " > WLAN DOWN"
ifconfig wlan0 down
echo " > WLAN SETUP"
ifconfig wlan0 10.0.0.1 netmask 255.255.255.0
iwconfig wlan0 mode ad-hoc
iwconfig wlan0 essid "Roboter CAN"
echo " > WLAN UP"
ifconfig wlan0 up

echo " > DHCPD START"
sleep 0.5
systemctl start dhcpd4' > /root/can-wlan-setup

chmod 0755 /root/can-wlan-setup

# CREATE AND ENABLE AUTOSTART SCRIPT
echo '#!/bin/bash

/root/can-wlan-setup
/usr/share/nginx/html/bin/can-handler - 2 0' > /root/can-autostart

chmod 0755 /root/can-autostart

echo '[Unit]
Description=Roboter CAN autostart script

[Service]
Type=idle
ExecStart=/root/can-autostart

[Install]
WantedBy=multi-user.target' > /etc/systemd/system/robotercan.service

echo '[Unit]
Description=Roboter CAN server autostart script

[Service]
Type=idle
ExecStart=/usr/share/nginx/html/bin/can-server

[Install]
WantedBy=multi-user.target' > /etc/systemd/system/robotercanServer.service

chmod 0755 /etc/systemd/system/robotercan.service
chmod 0755 /etc/systemd/system/robotercanServer.service

systemctl enable robotercan.service robotercanServer.service

# INSTALL WEBSERVER
pacman -S nginx php-fpm --noconfirm

# CONFIGURE NGINX
echo '
#user html;
worker_processes  1;

#error_log  logs/error.log;
#error_log  logs/error.log  notice;
#error_log  logs/error.log  info;

#pid        logs/nginx.pid;


events {
    worker_connections  1024;
}


http {
    include       mime.types;
    default_type  application/octet-stream;

    #log_format  main  '"'"'$remote_addr - $remote_user [$time_local] "$request" $status $body_bytes_sent "$http_referer" "$http_user_agent" "$http_x_forwarded_for"'"'"';

    #access_log  logs/access.log  main;

    sendfile        on;
    #tcp_nopush     on;

    #keepalive_timeout  0;
    keepalive_timeout  65;

    #gzip  on;

    server {
        listen       80;
        server_name  robotercan;

        include php.conf;

        #charset koi8-r;

        #access_log  logs/host.access.log  main;

        location / {
            root   /usr/share/nginx/html;
            index  index.html index.htm index.php index.php3 index.php4 index.php5;
            allow 10.0.0.1;
            allow 10.0.0.2;
            deny all;
        }

        #error_page  404              /404.html;

        # redirect server error pages to the static page /50x.html
        #
        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   /usr/share/nginx/html;
        }

        # proxy the PHP scripts to Apache listening on 127.0.0.1:80
        #
        #location ~ \.php$ {
        #    proxy_pass   http://127.0.0.1;
        #}

        # pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000

        location ~ \.php$ {
            root           /usr/share/nginx/html;
            fastcgi_pass   unix:/run/php-fpm/php-fpm.sock;
            fastcgi_index  index.php;
        #    fastcgi_param  SCRIPT_FILENAME  /scripts$fastcgi_script_name;
        #    include        fastcgi_params;
            include fastcgi.conf;
        }

        # deny access to .htaccess files, if Apaches document root
        # concurs with nginxs one
        #
        #location ~ /\.ht {
        #    deny  all;
        #}
    }


    # another virtual host using mix of IP-, name-, and port-based configuration
    #
    #server {
    #    listen       8000;
    #    listen       somename:8080;
    #    server_name  somename  alias  another.alias;

    #    location / {
    #        root   html;
    #        index  index.html index.htm;
    #    }
    #}


    # HTTPS server
    #
    #server {
    #    listen       443;
    #    server_name  localhost;

    #    ssl                  on;
    #    ssl_certificate      cert.pem;
    #    ssl_certificate_key  cert.key;

    #    ssl_session_timeout  5m;

    #    ssl_protocols  SSLv2 SSLv3 TLSv1;
    #    ssl_ciphers  HIGH:!aNULL:!MD5;
    #    ssl_prefer_server_ciphers   on;

    #    location / {
    #        root   html;
    #        index  index.html index.htm;
    #    }
    #}

}' > /etc/nginx/nginx.conf

# CREATE /etc/nginx/php.conf
echo '' > /etc/nginx/php.conf

# ADD USER HTTP TO GROUP ROOT
usermod -aG root http

# DIRECTORY CHANGING
sed -i 's/open_basedir = \/srv\/http/open_basedir = \/usr\/share\/nginx\/html/g' /etc/php/php.ini

# PHP-FPM FIX: SHUTDOWN NOT HANGING
echo '[Unit]
Description=The PHP FastCGI Process Manager
After=syslog.target network.target

[Service]
Type=notify
PIDFile=/run/php-fpm/php-fpm.pid
PrivateTmp=true
ExecStart=/usr/bin/php-fpm --daemonize --pid /run/php-fpm/php-fpm.pid
ExecReload=/bin/kill -USR2 $MAINPID

[Install]
WantedBy=multi-user.target' > /usr/lib/systemd/system/php-fpm.service

# ENABLE AUTOSTART FOR WEBSERVER
systemctl enable nginx php-fpm

# GETTY FIX: NO SCREEN CLEARING AT LOGIN
echo '#  This file is part of systemd.
#
#  systemd is free software; you can redistribute it and/or modify it
#  under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation; either version 2.1 of the License, or
#  (at your option) any later version.

[Unit]
Description=Getty on %I
Documentation=man:agetty(8) man:systemd-getty-generator(8)
Documentation=http://0pointer.de/blog/projects/serial-console.html
After=systemd-user-sessions.service plymouth-quit-wait.service

# If additional gettys are spawned during boot then we should make
# sure that this is synchronized before getty.target, even though
# getty.target didnt actually pull it in.
Before=getty.target
IgnoreOnIsolate=yes

# On systems without virtual consoles, dont start any getty. Note
# that serial gettys are covered by serial-getty@.service, not this
# unit.
ConditionPathExists=/dev/tty0

[Service]
# the VT is cleared by TTYVTDisallocate
ExecStart=-/sbin/agetty --noclear %I
Type=idle
Restart=always
RestartSec=0
UtmpIdentifier=%I
TTYPath=/dev/%I
TTYReset=yes
TTYVHangup=yes
TTYVTDisallocate=no
KillMode=process
IgnoreSIGPIPE=no
SendSIGHUP=yes

# Unset locale for the console getty since the console has problems
# displaying some internationalized messages.
Environment=LANG= LANGUAGE= LC_CTYPE= LC_NUMERIC= LC_TIME= LC_COLLATE= LC_MONETARY= LC_MESSAGES= LC_PAPER= LC_NAME= LC_ADDRESS= LC_TELEPHONE= LC_MEASUREMENT= LC_IDENTIFICATION=

[Install]
WantedBy=getty.target' > /etc/systemd/system/getty.target.wants/getty@tty1.service

# UPDATE BOOTLOADER AND ENABLE CAMERA
echo '# uncomment if you get no picture on HDMI for a default "safe" mode
#hdmi_safe=1

# uncomment this if your display has a black border of unused pixels visible
# and your display can output without overscan
disable_overscan=1

# uncomment the following to adjust overscan. Use positive numbers if console
# goes off screen, and negative if there is too much border
#overscan_left=16
#overscan_right=16
#overscan_top=16
#overscan_bottom=16

# uncomment to force a console size. By default it will be displays size minus
# overscan.
#framebuffer_width=1280
#framebuffer_height=720

# uncomment if hdmi display is not detected and composite is being output
#hdmi_force_hotplug=1

# uncomment to force a specific HDMI mode (this will force VGA)
#hdmi_group=1
#hdmi_mode=1

# uncomment to force a HDMI mode rather than DVI. This can make audio work in
# DMT (computer monitor) modes
#hdmi_drive=2

# uncomment to increase signal to HDMI, if you have interference, blanking, or
# no display
#config_hdmi_boost=4

# uncomment for composite PAL
#sdtv_mode=2

#uncomment to overclock the arm. 700 MHz is the default.
#arm_freq=800

# for more options see http://elinux.org/RPi_config.txt

## Some over clocking settings, govenor already set to ondemand

##None
#arm_freq=700
#core_freq=250
#sdram_freq=400
#over_voltage=0

##Modest
#arm_freq=800
#core_freq=300
#sdram_freq=400
#over_voltage=0

##Medium
#arm_freq=900
#core_freq=333
#sdram_freq=450
#over_voltage=2

##High
#arm_freq=950
#core_freq=450
#sdram_freq=450
#over_voltage=6

##Turbo
#arm_freq=1000
#core_freq=500
#sdram_freq=500
#over_voltage=6

gpu_mem_512=316
gpu_mem_256=128
cma_lwm=16
cma_hwm=32
cma_offline_start=16

start_file=start_x.elf
fixup_file=fixup_x.dat' > /boot/config.txt

# CAMERA PERMISSION FIX
echo 'SUBSYSTEM=="vchiq",GROUP="video",MODE="0660"' > /etc/udev/rules.d/10-vchiq-permissions.rules
usermod -aG video http

# INSTALL GCC TO COMPILE HANDLER, ETC.
pacman -S gcc --noconfirm

# NEW ROOT PASSWORD
passwd root

# INSTALL GIT FOR WIRINGPI
pacman -S git make --noconfirm

# INSTALL WIRINGPI FROM GIT REPOSITORY
cd /opt
git clone git://git.drogon.net/wiringPi
cd wiringPi/
git pull origin

# FIX BUILD SCRIPT FOR WIRING PI
echo '#!/bin/sh

check_make_ok() {
  if [ $? != 0 ]; then
    echo ""
    echo "Make Failed..."
    echo "Please check the messages and fix any problems. If youre still stuck,"
    echo "then please email all the output and as many details as you can to"
    echo "  projects@drogon.net"
    echo ""
    exit 1
  fi
}

if [ x$1 = "xclean" ]; then
  cd wiringPi
  echo -n "wiringPi:   "	; make clean
  cd ../devLib
  echo -n "DevLib:     "	; make clean
  cd ../gpio
  echo -n "gpio:       "	; make clean
  cd ../examples
  echo -n "Examples:   "	; make clean
  cd Gertboard
  echo -n "Gertboard:  "	; make clean
  cd ../PiFace
  echo -n "PiFace:     "	; make clean
  cd ../q2w
  echo -n "Quick2Wire: "	; make clean
  cd ../PiGlow
  echo -n "PiGlow:     "	; make clean
  exit
fi

if [ x$1 = "xuninstall" ]; then
  cd wiringPi
  echo -n "wiringPi: " ; sudo make uninstall
  cd ../devLib
  echo -n "DevLib:   " ; sudo make uninstall
  cd ../gpio
  echo -n "gpio:     " ; sudo make uninstall
  exit
fi


  echo "wiringPi Build script"
  echo "====================="
  echo

  echo
  echo "WiringPi Library"
  cd wiringPi
  make uninstall
  if [ x$1 = "xstatic" ]; then
    make static
    check_make_ok
    make install-static
  else
    make
    check_make_ok
    make install
  fi
  check_make_ok

  echo
  echo "WiringPi Devices Library"
  cd ../devLib
  make uninstall
  if [ x$1 = "xstatic" ]; then
    make static
    check_make_ok
    make install-static
  else
    make
    check_make_ok
    make install
  fi
  check_make_ok

  echo
  echo "GPIO Utility"
  cd ../gpio
  make
  check_make_ok
  make install
  check_make_ok

# echo
# echo "Examples"
# cd ../examples
# make
# cd ..

echo
echo All Done.
echo ""
echo "NOTE: This is wiringPi v2, and if you need to use the lcd, Piface,"
echo "  Gertboard, MaxDetext, etc. routines then you must change your"
echo "  compile scripts to add -lwiringPiDev"
echo ""' > /opt/wiringPi/build

# COMPILE WIRING PI
/opt/wiringPi/build

# TEST WIRING PI INSTALLATION
gpio -v

# INSTALL RSYNC FOR BACKUPS
pacman -S rsync --noconfirm

# PREPARE CRON JOB FOR REFRESHING TEMPERATURE
mkdir -p /usr/share/nginx/html/bin/cpuValues
echo '<?php
	// this script will output the CORE_TEMP VORE_VOLTS and ARM_CLOCK (in this order)
	// into the specified file, used for handler and fan-control
	
	// THE CONTENT OF THIS FILE WILL BE UPDATED SOON (upload the code to the robot)
?>' > /usr/share/nginx/html/bin/can-temp-refresh.php

# CREATE CRON JOB FOR REFRESHING TEMPERATURE
echo '# cron job for temperature refresh

SHELL=/bin/sh
PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin

# m h dom mon dow user	command
* * * * * root php -f /usr/share/nginx/html/bin/can-handler - 15 0' > /etc/cron.d/1temp

# PRINT OUT RECOMMENDED ACTIONS
echo "

It is recommended to reboot NOW! There are several changes in depth which are only applied on reboot.
"
