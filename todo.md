# GnuChanWM: Openbox kaynak incelemesine dayali gelistirme listesi

Bu liste, `C:\Users\btw\Documents\openbox\openbox` altindaki Openbox kaynaklari
ile `src/` altindaki GnuChanWM kaynaklarinin karsilastirilmasindan cikmistir.
Amac Openbox'in butun davranisini kopyalamak degil; X11/ICCCM/EWMH
sozlesmelerini dogru uygulamak, uygulama girisini WM'den bagimsiz kilmak ve
her duzeltmeyi gercek istemci olaylariyla sinamaktir. Sure tahmini bilerek
yoktur.

## Mevcut kaynakta dogrulanmis bulgular

- [x] Sentetik `ConfigureNotify` olayinda istemciye frame-ici koordinat yerine
  root'a gore koordinat bildirilmesi. Openbox `client.c` bu olayi
  `self->root_pos` ile kuruyor; GnuChanWM `frame_notify_configure()` artik
  frame konumunu ve istemci offset'ini kullaniyor.
- [x] Yoneticinin cerceve olculerinin `_NET_FRAME_EXTENTS` ile bildirilmesi.
  Openbox `frame.c` bu property'yi istemcide yayinliyor; GnuChanWM artik
  property'yi ilan ediyor, yaziyor ve istemci serbest birakilirken kaldiriyor.
- [x] Uygulamanin secmedigi button olaylarinin root'a yayilip masaustu
  tiklamasi olarak tuketilmesi riskinin giderilmesi. Openbox
  `CLIENT_NOPROPAGATEMASK` kullanir; GnuChanWM istemci penceresinde button
  press/release/motion yayilmasini engelliyor.
- [x] WM kapanirken/reparent ederken istemcinin root konumunun frame
  dekorasyon offset'leriyle korunmasi.
- [ ] Bu degisiklikler icin kalici Xvfb testleri ekle. Daha once yapilan Xvfb
  kontrolleri tek seferlikti; kaynak degisikliklerinden sonra otomatik
  calismiyor ve Wine davranisinin yerine gecmiyor.

## P0 — Wine ve temel fare girisi: once bunu bitir

- [ ] `tests/` altinda minimal Xlib test istemcisi olustur: pencere icinde
  farkli y/yukseklik bolgelerine tiklamayi kaydetsin, pointer koordinatini,
  `ConfigureNotify` koordinatini ve aldigi ButtonPress koordinatini loglasin.
- [ ] Xvfb altinda WM'yi baslatip istemciyi yonet: istemci root koordinati
  `XTranslateCoordinates(client, root, 0, 0)` ile olculen gercek konumla
  WM'nin son `ConfigureNotify` x/y degerleri esit olmali. Pencere tasininca,
  boyutlaninca, maximize/restore olunca da esitlik tekrar dogrulanmali.
- [ ] Xvfb testinde `_NET_FRAME_EXTENTS` turunun `CARDINAL`, formatinin 32 ve
  dort degerinin sirasiyla left, right, top, bottom oldugunu kontrol et.
  Client tasinirken extents degismemeli; frame stili/border degisince
  guncellenmeli; unmanage/WM cikisinda property silinmeli.
- [ ] Uygulama tarafinda secilmeyen button olaylarini test et. Bir client
  alani ile root arasinda propagation olmasin; client'in kendi secip
  isledigi click ise WM tarafindan engellenmesin. Test, WM masaustu
  tiklamasinin client click'ini yutmadigini da kanitlamali.
- [ ] `Alt+sol` surukleme ve `Alt+sag` resize WM tarafindan yakalansin;
  modifiersiz press/release/motion client'e dogrudan ulassin. Grab basarisi,
  baslangic/bitis ve WM cikisinda grab'in serbest kalmasi test edilsin.
- [ ] Wine penceresinin alt pencerelerine (widget/child window) tiklama
  senaryosu ekle. Client'in ust seviyesi ile child penceresi arasinda
  koordinat donusumu veya event propagation farki olmamali.
- [ ] Wine popup/menu senaryosunu test et: override-redirect popup normalde
  WM tarafindan cercevelenmeden kendi konumunda acilmali; popup icindeki
  click ve disaridaki dismiss olayi calismali.
- [ ] Gercek oturum kabul testi: winecfg, Winefile, setup.exe inputbox ve
  TLauncher surum secimi. Her birinde imlecin gorunmesi, imlecin gosterdigi
  yere tiklanmasi, popup'un acik kalmasi ve Tab/klavye ile odagin
  tutarliligi kaydedilmeli. Xvfb bu testi ikame etmez.
- [ ] Onceki hata tekrar uretilirse ayni test matrisi Openbox oturumunda da
  calistirilsin. Ekran cozunurlugu, fractional/DPI scaling, Xorg/Xwayland,
  Wine surumu ve compositor durumu kaydedilsin; GnuChanWM ile Openbox ayni
  ortamda karsilastirilsin.

## P1 — X11 pencere yasam dongusu ve geometri

- [ ] `wm_manage.c::manage_configure()` davranisini ICCCM'ye gore tamamla.
  Su an yonetilen pencere icin boyut aliniyor, konum reddediliyor; sibling ve
  stack mode, border width ve gravity istekleri genel durumda uygulanmiyor.
  Her maske biti icin beklenen davranisi tanimla; client'in kendi konumunu
  WM frame'inden ayir; boyut ve stacking istegini koru.
- [ ] `WM_NORMAL_HINTS` uygulamasini tek bir dogrulanmis geometri yoluna
  indir: min/max, base size, resize increments ve aspect oranlari hem client
  ConfigureRequest'lerinde hem WM drag/maximize isteklerinde tutarli
  uygulanmali. Su an resize increments okunuyor; min/max/aspect'in tum
  yollarla uyumu eksik.
- [ ] `PWinGravity` ve StaticGravity davranisini test et. Reparent, frame
  border degisimi, client resize ve WM restart sirasinda client'in bekledigi
  referans noktasi kaymamali.
- [ ] Yeni pencere konumlandirmasini Openbox `client_manage()` placement
  akisiyla karsilastir: `USPosition`/`PPosition`, ekran disina tasan pencere,
  transient/modal pencere ve monitor secimi icin acik politika belirle.
  Programin konum istegini her durumda yok saymak yerine hint'e gore karar
  ver.
- [ ] `ConfigureNotify` tek kaynaktan ve ICCCM kurallarina uygun uretilsin.
  Gercek sunucu olaylariyla sentetik yanitlar ayrilsin; `send_event`,
  root-relative x/y, border, above ve resize bildirimi test edilsin. Bir
  bildirim WM tarafinda yeni configure istegi gibi ele alinip dongu
  baslatmamali.
- [ ] Pencere olusturma/yok etme, map/unmap, reparent ve WM'nin yeniden
  baslamasi sirasindaki `ignore_unmaps` sayacini olay turu ve window ID
  bazinda dogrula. Tek global sayaç sirasi degismis event'lerde yanlis
  istemciyi korumamali.
- [ ] WM basladiginda root altindaki mevcut pencereleri yonetme yolunu
  koruyup test et (`manage_init()` XQueryTree yapiyor). Map state,
  override-redirect, InputOnly, destroy/map yarisi ve baska WM'den devir
  senaryolari kapsansin.
- [ ] Unmanage/WM cikisinda client'i kaydetmeden once EWMH/ICCCM durumlarini
  temizle: `_NET_FRAME_EXTENTS`, `_NET_WM_DESKTOP`, WM_STATE, passive grabs,
  save set ve root koordinatli reparent. Client yasiyorsa X sunucusunda
  artik WM frame'ine bagli grab veya stale property kalmamali.

## P1 — Giris odagi ve pencere iliskileri

- [ ] Focus politikasini acikca secilebilir hale getir: click-to-focus ile
  focus-follows-mouse ayrilsin. Su an `EnterNotify` ve `ButtonPress` ayni
  focus yoluna giriyor; pencere/child gecislerinde `mode`, `detail`,
  unmap/destroy ve popup odagi test edilmeli.
- [ ] Odak degisimini Openbox `focus.c` akisi gibi tek ve atomik bir
  operasyon yap: pencereyi gorunur/aktif kil, raise/stack kararini ver,
  `WM_HINTS InputHint` ve `WM_TAKE_FOCUS` protokollerini uygula,
  `_NET_ACTIVE_WINDOW` ve visual focus'u esle. Root'a focus ve
  focus-history guncellemeleri de ayni politikayi izlesin.
- [ ] `WM_PROTOCOLS` property degisikliklerini isle. `WM_TAKE_FOCUS` ve
  `WM_DELETE_WINDOW` listesini sadece ilk manage aninda degil, client
  protokol listesini degistirdiginde de guncel kullan.
- [ ] `WM_TRANSIENT_FOR` iliskilerini yonet: dialog/prompt parent'in
  workspace ve stacking ailesinde kalmali; parent minimize/unmanage
  edilince davranis tanimli olmali; modal dialog parent'i etkisiz kilmali.
- [ ] `WM_HINTS` icindeki input tercihi disinda urgency/state, icon ve
  group/client leader bilgisini dikkate al. Openbox `client_get_all()` ve
  hint yenileme yollarini referans al.
- [ ] Override-redirect, dock, splash, desktop, notification, tooltip,
  dialog ve menu turlerini birbirinden ayir. Su an unmanaged tur kontrolu
  esasen dock/splash ve override-redirect ile sinirli; her EWMH pencere
  turu icin yonetim, stacking ve odak politikasi yaz.

## P1 — Event mimarisi ve hata ayiklama

- [ ] Olaylari tum modullere sirayla broadcast etmek yerine window ID/event
  turu bazinda merkezi dispatch tablosu kur. Su an `core_dispatch()` her
  eventi her modula yollar; handler sirasi ve birden fazla handler'in ayni
  olayi degistirmesi protokol davranisini kirilgan yapabilir.
- [ ] Openbox `event_get_window()`/window map yaklasimina benzer sekilde
  frame, client, client child, desktop, menu, tray ve support window
  sahipligini kaydet. Her Button/Enter/Configure olayinda tekrar tekrar
  `XQueryTree` ile tahmin yurutmek yerine kayitli sahiplikten cozumle.
- [ ] X hata yoneticisini kapsamli hale getir: `XGrabButton`,
  `XGrabPointer`, `XSelectInput`, `XChangeWindowAttributes` ve window
  destroy yarislari icin hata kodu, request ve resource ID loglansin.
  Hata sessizce basarili kabul edilmesin; kritik grab basarisizsa
  davranis acikca raporlansin.
- [ ] Debug secenegi ekle: event type/window, crossing mode/detail,
  button/state, root ve local koordinat, pointer grab sonucu, focus target,
  configure request mask ve synthetic/real ayrimini tek satirlik logla.
  Varsayilan oturumda log gurultusu yaratmasin.
- [ ] X server olmadan calisan geometri/protokol unit testleri ile Xvfb
  entegrasyon testlerini ayir; derleme betiginden tek komutla
  calistirilabilsin. Wine kabul testi ayri/manual olarak belgelenmeli.

## P2 — Openbox benzeri EWMH ve uygulama uyumlulugu

- [ ] `_NET_SUPPORTED` yalnizca gercekten uygulanan atomlari ilan etsin.
  `_NET_FRAME_EXTENTS` eklendi; simdi ilan edilen tum atomlar icin istemci
  davranisi ve dogru mesaj/property uygulamasi test edilsin.
- [ ] `_NET_REQUEST_FRAME_EXTENTS` istegini isle; pencere olusturulmadan
  once de istemciye kullanilacak dekorasyon olculerini bildirebil.
- [ ] `_NET_WM_MOVERESIZE` ve client kaynakli move/resize taleplerini
  uygulamaya al. EWMH yonleri, klavye tasima, pointer tasima ve cancel
  davranisi tanimli olsun.
- [ ] `_NET_CLOSE_WINDOW`, `WM_CHANGE_STATE`, `_NET_WM_STATE` add/remove/
  toggle ve `_NET_ACTIVE_WINDOW` istemci mesajlarini dogru kaynak/timestamp
  kontrolleriyle isle. Desteklenmeyen action sessizce basarili gorunmesin.
- [ ] `_NET_WM_STATE` listesini tek bir state olarak ezmek yerine bilinmeyen
  state atomlarini koru; sadece WM'nin yonettigi fullscreen/maximize
  atomlarini degistir. PropertyNotify ve ClientMessage ayni sonucu vermeli.
- [ ] EWMH stacking/client list/desktop property'lerini atomik ve dogru
  sirada yayinla: `_NET_CLIENT_LIST_STACKING`, `_NET_WM_DESKTOP`,
  `_NET_CURRENT_DESKTOP`, desktop sayisi ve active window degisimleri
  pager/taskbar ile test edilmeli.
- [ ] Maximize yatay/dikey, fullscreen ve restore state'lerini bagimsiz
  uygula. Simdiki frame davranisindaki “fullscreen chrome hiisen” gibi
  urune ozel kararlar EWMH sozlesmesinden ayrica, kullanici tercihi olarak
  tanimlansin; oyun konumunu her zaman reddetmek varsayilan genel kural
  olmasin.

## P2 — Frame, stacking ve ekran duzeni

- [ ] Openbox `frame.c` gibi istemciyi dekorasyon alt pencereleriyle
  ayir: titlebar, kenarlik, resize hit-area ve client ayri X pencereleri
  olsun. GnuChanWM'nin tek frame penceresine hem buton hem client disi
  event-mask gorevi yukleme; click koordinatinin hangi yuzeye ait oldugu
  X11 agacinda acik olsun.
- [ ] Stack/focus modelini tek listeyle yonet. Click-to-raise, always-on-top,
  transient/dialog ailesi ve workspace gorunurlugu `XRaiseWindow()` cagri
  daginikligindan bagimsiz test edilsin.
- [ ] Monitor/WorkArea hesabini RandR degisimlerinde guncelle. Pencere,
  maximized alan, desktop bar ve yeni placement birden fazla monitor,
  monitor ekle/cikar ve rotation senaryolarinda uyumlu olsun.
- [ ] Pencereyi ekrana sigdirirken istemcinin talep ettigi boyutu keyfi
  kirpmak yerine size hints ve kullanici tercihine gore davran. Buyuk
  pencerede title bar erisilebilir kalmali; icerik kesilmesi,
  bilerek fullscreen olan oyun ve DPI farki ayri senaryolarda olculmeli.
- [ ] Resize hit-area/cursor davranisini Openbox'in ayri edge/corner
  frame window'lariyla karsilastir. Kenar ve koselerde dogru resize cursor,
  button routing ve Alt+resize ile cakismazlik sagla.

## P2 — Compositor ve cursor siniri

- [ ] WM ile compositor sorumluluklarini ayir. Uygulama pencerelerinin
  normal yolu X sunucusunun dogrudan cizimi ve dogrudan input'u olsun;
  compositor redirect/scale varsayilan pencere yonetim yoluna girmesin.
- [ ] Olceklenmis pencere ozelligi tutulacaksa bunu deneysel/explicit
  ozellik olarak ayir; XRender'a gore ters pointer koordinati, child
  windows, XInput2, cursor hotspot, popup ve relative pointer davranisi
  eksiksiz uygulanmadan oyun/Wine penceresine otomatik uygulama.
- [ ] Client cursor'larini ezme. Root cursor temasi WM masaustu ve kendi
  chrome pencerelerinde uygulanmali; uygulamanin client/child penceresinde
  tanimladigi cursor ve Wine'in gizli/relative cursor davranisi
  korunmali.
- [ ] Compositor redirect/unredirect, damage, client destroy, screen
  suspend/resume ve WM shutdown senaryolarina Xvfb/Xorg testleri ekle.
  Pencerenin donmus/son frame'de kalmis/gorunmez olmasi kabul edilmemeli.

## P3 — Uygulama ve surum kalitesi

- [ ] `tests/` ve CI ekle: GCC uyarilari, format/diff check, geometri unit
  testleri, Xvfb entegrasyon testleri ve Debian build. CI basarisi Wine'in
  canli uyumlulugunu kanitlamaz; ayri kabul testi gereklidir.
- [ ] Kullaniciya verilecek debug raporunda GnuChanWM commit/build kimligi,
  X server tipi, display scale/DPI, Wine surumu, WM logu ve test sonucu
  toplansin. Icerik/kimlik bilgileri gibi gizli veriler loga alinmasin.
- [ ] Her protokol duzeltmesinde Openbox'i ayni test istemcisiyle referans
  davranis olarak calistir; Openbox'in urune ozel ozelliklerini degil,
  ICCCM/EWMH gozlemlenebilir sozlesmesini karsilastir.

## Kaynak karsilastirma haritasi

| Alan | Openbox referansi | GnuChanWM inceleme noktasi |
|---|---|---|
| Client event mask ve propagation | `openbox/client.c`: `CLIENT_EVENTMASK`, `CLIENT_NOPROPAGATEMASK`, `client_manage()` | `src/wm_frame.c`: client `XChangeWindowAttributes()` ve frame input mask |
| Frame ve client X agaci | `openbox/frame.c`: `frame_grab_client()`, client `(0,0)` reparent ve ayri frame elementleri | `src/wm_frame.c`: tek frame, client border/title offset'i |
| Root koordinatli configure | `openbox/client.c`: `client_reconfigure()`; sentetik `ConfigureNotify` root koordinatlari | `src/wm_frame.c`: `frame_notify_configure()` |
| Frame extents | `openbox/frame.c`: `_NET_FRAME_EXTENTS` / KDE strut yayinlama | `src/wm_frame.c`, `src/wm_core.c` |
| Mouse grabs/replay | `openbox/mouse.c`: client/frame context, `ReplayPointer`; `openbox/grab.c` | `src/wm_frame.c`: Alt grabs; `src/wm_frame_interact.c`: drag/resize |
| Event dispatch | `openbox/event.c`: event window cozumleme ve tur bazli yonlendirme | `src/wm_core.c::core_dispatch()`, tum module event handler'lari |
| Startup'ta mevcut pencereler | `openbox/openbox.c`, `openbox/window.c::window_manage_all()` | `src/wm_manage.c::manage_init()` |
| Hints ve transient | `openbox/client.c::client_get_all()` ve client hint/property yenileme | `src/wm_manage.c`, `src/wm_focus.c`, `src/wm_frame.c` |
| Configure istekleri | `openbox/event.c`, `openbox/client.c::client_configure()` | `src/wm_manage.c::manage_configure()`, `src/wm_frame.c::wm_frame_resize()` |

## Bitmis sayilma kosulu

P0 Xvfb testlerinin hepsi otomatik gecmeden ve Wine kabul testi gercek Linux
oturumunda basarili olmadan fare/koordinat sorunu kapatilmis sayilmaz. Her
sonraki P1/P2 maddesi de kendi test ve kabul kosuluyla tamamlanmali; yalnizca
derlenmesi veya pencerenin gorunmesi pencere yoneticisinin dogru davrandigini
gostermez.
