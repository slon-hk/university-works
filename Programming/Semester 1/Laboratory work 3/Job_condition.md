Напишите программу, которая форматирует некоторый текст и выводит результат на консоль.

На вход программе подается текст, который заканчивается предложением "Dragon flew away!".

Предложение (кроме последнего) может заканчиваться на:

    . (точка)
    ; (точка с запятой)
    ? (вопросительный знак)

Программа должна изменить и вывести текст следующим образом:

    Каждое предложение должно начинаться с новой строки.
    Табуляция в начале предложения должна быть удалена.
    Все предложения, в которых есть цифры внутри слов, должны быть удалены (это не касается слов, которые начинаются/заканчиваются цифрами). Если слово начинается с цифры, но имеет и цифру в середине, удалять его все равно требуется (4a4a).
    Текст должен заканчиваться фразой "Количество предложений до n и количество предложений после m", где n - количество предложений в изначальном тексте (без учета терминального предложения "Dragon flew away!") и m - количество предложений в отформатированном тексте (без учета предложения про количество из данного пункта).

 

* Порядок предложений не должен меняться

* Статически выделять память под текст нельзя

* Пробел между предложениями является разделителем, а не частью какого-то предложения


Пример:

***Input*** 
Fusce finibus sapien magna,
quis scelerisque ex sodales tristique. 
Ut auctor augue vel tincidunt tincidunt 
555. Fusce finibus sapien magna, quis 
scelerisque ex sodales tristique. Ut auctor
augue vel tincidunt tincidunt 555. Aliquam
555 condimentum ligula arcu, non mollis 
ex pell555entesque finibus. Aenean magna 
massa, scelerisque quis sagittis at, 
pharetra a lectus? Ut auctor augue vel 
tincidunt tincidunt 555. Suspendisse quis 
mi neque7. 40 Nu555lla rutrum feugiat 
felis a pharetra. Aenean magna massa, 
scelerisque quis sagittis at, pharetra
a lectus? Integer lAoreet5 vene45natis
ullamcorper? Suspendisse quis mi neque7.
Phasellus nunc augue, dApibus quis molestie
iD, gravida a velit? Lorem ipsum dolor sit 
amet, consectetur adipiscing elit; Ut a7uctor
, leo eu dictum vestibulum, tortor enim 
consequat mauris, eget consectetur justo
quam et 7 metus. 1 Vivamus eu nibh rhoncus, 
da456pibus ex non, sodales mi. Morbi c
o7ndimentum 555 ex justo, nec pharetra 
mauris vestibulum a. Vivamus sit amet 
viverra arcu, sed ultricies nulla. 
Praesent egestas nunc mattis imperdiet
posuere. Donec at nunc ac mauris8 
suscipit venenatis. Etiam quis neque
tincidunt, porta odio vitae, scelerisque
ante? Nam 7elementum id enim eu congue;
Praesent egestas nunc mattis imperdiet
posuere. Sed finibus magna et mauris 
elementum tempus? Morbi co7ndimentum 555 
ex justo, nec pharetra mauris vestibulum a. 
1 Vivamus eu nibh rhoncus, da456pibus 
ex non, sodales mi. Class aptenT taciti 
sociosqu ad litora torquent per cOnubia 
nostra, per inceptos himenaeos. Phasellus
iaculis sem ac congue egestas. 1 Vivamus
eu nibh rhoncus, da456pibus ex non,
sodales mi. Integer lAoreet5 vene45natis
ullamcorper? Donec accumsan convallis 
ipsum vitae lacinia. Phasellus nunc augue,
dApibus quis molestie iD, gravida a velit?
Aenean magna massa, scelerisque quis 
sagittis at, pharetra a lectus? Nu555llam
auctor vehicula dui, quis lobortis nibh.
Nam 7elementum id enim eu congue; 
Phasellus nunc augue, dApibus quis 
molestie iD, gravida a velit? Aenean 
magna massa, scelerisque quis sagittis
at, pharetra a lectus? Ut auctor augue 
vel tincidunt tincidunt 555. Donec at 
nunc ac mauris8 suscipit venenatis. 
Integer at quam et erat iaculis iaculis 
hendrerit a te4llus? Phasellus nunc 
augue, dApibus quis molestie iD, gravida
a velit? Suspendisse quis mi neque7.
Lorem ipsum dolor sit amet, consectetur
adipiscing elit. Donec congue mauris sed
lacus pulvinar, quis semper orci s
ol4licitudin? Integer at quam et erat 
iaculis iaculis hendrerit a te4llus? 
Aenean magna massa, scelerisque quis
sagittis at, pharetra a lectus? 40 
Nu555lla rutrum feugiat felis a pharetra.
Donec at nunc ac mauris8 suscipit 
venenatis. Maecenas 555 posuere velit 
efficitur, egestas nunc quis, dictum 
purus? Pellentesque eu augue malesuada,
condimenTum nisl id, cur4sus 555 turpis.
Morbi co7ndimentum 555 ex justo, nec 
pharetra mauris vestibulum a. Etiam 
quis neque tincidunt, porta odio vitae,
scelerisque ante? Fusce finibus sapien
magna, quis scelerisque ex sodales 
tristique. Fusce finibus sapien magna,
quis scelerisque ex sodales tristique.
Suspendisse quis mi neque7. Aenean 
sem ligula, laoreet ac sodales a, 
congue euismod neque; 40 Nu555lla 
rutrum feugiat felis a pharetra.
1 Vivamus eu nibh rhoncus, da456pibus 
ex non, sodales mi. Fusce finibus 
sapien magna, quis scelerisque ex 
sodales tristique. Class aptenT taciti
sociosqu ad litora torquent per cOnubi
a nostra, per inceptos himenaeos. 
Maecenas 555 posuere velit efficitur,
egestas nunc quis, dictum purus? 
Phasellus nunc augue, dApibus quis
molestie iD, gravida a velit? Nam 
7elementum id enim eu congue; Ut a7uctor,
leo eu dictum vestibulum, tortor enim 
consequat mauris, eget consectetur 
justo quam et 7 metus. 1 Vivamus eu 
nibh rhoncus, da456pibus ex non,
sodales mi. 40 Nu555lla rutrum feugiat 
felis a pharetra. Donec congue mauris 
sed lacus pulvinar, quis semper orci 
sol4licitudin? Etiam quis neque tincidunt,
porta odio vitae, scelerisque ante? 
Integer at quam et erat iaculis iaculis
hendrerit a te4llus? 1 Vivamus eu nibh
rhoncus, da456pibus ex non, sodales mi.
Nam 7elementum id enim eu congue; Nam 
7elementum id enim eu congue; Integer 
at quam et erat iaculis iaculis 
hendrerit a te4llus? Morbi co7ndimentum
555 ex justo, nec pharetra mauris 
vestibulum a. Pellentesque eu augue 
malesuada, condimenTum nisl id, cur4sus
555 turpis. Morbi co7ndimentum 555 ex 
justo, nec pharetra mauris vestibulum a.
Nam 7elementum id enim eu congue;
Maecenas 555 posuere velit efficitur,
egestas nunc quis, dictum purus? 
Integer lAoreet5 vene45natis 
ullamcorper? Aliquam 555 condimentum
ligula arcu, non mollis ex 
pell555entesque finibus. Phasellus
nunc augue, dApibus quis molestie iD,
gravida a velit? Donec congue mauris 
sed lacus pulvinar, quis semper orci
sol4licitudin? Fusce finibus sapien
magna, quis scelerisque ex sodales 
tristique. Praesent egestas nunc 
mattis imperdiet posuere. Cras eget
felis nibh? Nulla facilisi. 
Pellentesque eu augue malesuada,
condimenTum nisl id, cur4sus 555 
turpis. Etiam quis neque tincidunt,
porta odio vitae, scelerisque ante?
Sed finibus magna et mauris 
elementum tempus? Nam 7elementum 
id enim eu congue; Ut auctor augue 
vel tincidunt tincidunt 555. Praesent
egestas nunc mattis imperdiet posuere.
Donec congue mauris sed lacus pulvinar,
quis semper orci sol4licitudin? Donec
accumsan convallis ipsum vitae lacinia.
1 Vivamus eu nibh rhoncus, da456pibus 
ex non, sodales mi. Aenean sem ligula,
laoreet ac sodales a, congue euismod
neque; Donec accumsan convallis ipsum
vitae lacinia. Donec at nunc ac 
mauris8 suscipit venenatis. Integer 
lAoreet5 vene45natis ullamcorper? 
Integer lAoreet5 vene45natis 
ullamcorper? Nulla facilisi. 
Nu555llam auctor vehicula dui, quis
lobortis nibh. 40 Nu555lla rutrum 
feugiat felis a pharetra. Donec 
accumsan convallis ipsum vitae 
lacinia. Fusce finibus sapien magna,
quis scelerisque ex sodales tristique.
Lorem ipsum dolor sit amet, consectetur
adipiscing elit. Ut auctor augue vel 
tincidunt tincidunt 555. Phasellus 
nunc augue, dApibus quis molestie iD,
gravida a velit? Lorem ipsum dolor
sit amet, consectetur adipiscing elit.
Lorem ipsum dolor sit amet, consectetur
adipiscing elit. Ut a7uctor, leo eu 
dictum vestibulum, tortor enim consequat
mauris, eget consectetur justo quam et
7 metus. Phasellus nunc augue, dApibus 
quis molestie iD, gravida a velit? 
Suspendisse quis mi neque7. Suspendisse 
quis mi neque7. Lorem ipsum dolor sit 
amet, consectetur adipiscing elit. Donec
accumsan convallis ipsum vitae lacinia.
Etiam quis neque tincidunt, porta odio 
vitae, scelerisque ante? Integer lAoreet5
vene45natis ullamcorper? Donec congue 
mauris sed lacus pulvinar, quis semper 
orci sol4licitudin? Ut a7uctor, leo eu 
dictum vestibulum, tortor enim consequat 
mauris, eget consectetur justo quam et 7
metus. Suspendisse quis mi neque7. 
Maecenas 555 posuere velit efficitur,
egestas nunc quis, dictum purus? Maecenas
555 posuere velit efficitur, egestas nunc
quis, dictum purus? Aliquam 555 
condimentum ligula arcu, non mollis ex 
pell555entesque finibus. Ut auctor augue 
vel tincidunt tincidunt 555. Donec at 
nunc ac mauris8 suscipit venenatis. Aenean
magna massa, scelerisque quis sagittis at,
pharetra a lectus? Cras eget felis nibh? 
Suspendisse quis mi neque7. Donec accumsan 
convallis ipsum vitae lacinia. Nulla facilisi.
Aliquam 555 condimentum ligula arcu, non 
mollis ex pell555entesque finibus. Lorem 
ipsum dolor sit amet, consectetur adipiscing
elit. Morbi co7ndimentum 555 ex justo, nec 
pharetra mauris vestibulum a. Pellentesque 
eu augue malesuada, condimenTum nisl id, 
cur4sus 555 turpis. Maecenas 555 posuere 
velit efficitur, egestas nunc quis, dictum 
purus? Phasellus nunc augue, dApibus quis
molestie iD, gravida a velit? Praesent 
egestas nunc mattis imperdiet posuere. Nam
7elementum id enim eu congue; Donec accumsan
convallis ipsum vitae lacinia. Etiam quis
neque tincidunt, porta odio vitae, 
scelerisque ante? Phasellus iaculis sem 
ac congue egestas. Aenean magna massa,
scelerisque quis sagittis at, pharetra a 
lectus? Lorem ipsum dolor sit amet, 
consectetur adipiscing elit; Suspendisse
quis mi neque7. Maecenas 555 posuere velit
efficitur, egestas nunc quis, dictum purus?
Phasellus nunc augue, dApibus quis molestie 
iD, gravida a velit? Aenean magna massa,
scelerisque quis sagittis at, pharetra a 
lectus? Sed finibus magna et mauris
elementum tempus? Donec accumsan convallis 
ipsum vitae lacinia. Donec congue mauris
sed lacus pulvinar, quis semper orci 
sol4licitudin? 1 Vivamus eu nibh rhoncus,
da456pibus ex non, sodales mi. Maecenas 
555 posuere velit efficitur, egestas nunc 
quis, dictum purus? Sed finibus magna et
mauris elementum tempus? 1 Vivamus eu
nibh rhoncus, da456pibus ex non, sodales
mi. Ut a7uctor, leo eu dictum vestibulum, 
tortor enim consequat mauris, eget 
consectetur justo quam et 7 metus. Nulla
facilisi. Donec at nunc ac mauris8 suscipit 
venenatis. Dragon flew away!

***Result**
Fusce finibus sapien 
magna, quis scelerisque ex sodales tristique.
Ut auctor augue vel tincidunt tincidunt 555.
Fusce finibus sapien magna, quis scelerisque 
ex sodales tristique.
Ut auctor augue vel tincidunt tincidunt 555.
Aenean magna massa, scelerisque quis sagittis 
at, pharetra a lectus?
Ut auctor augue vel tincidunt tincidunt 555.
Suspendisse quis mi neque7.
Aenean magna massa, scelerisque quis sagittis
at, pharetra a lectus?
Suspendisse quis mi neque7.
Phasellus nunc augue, dApibus quis molestie 
iD, gravida a velit?
Lorem ipsum dolor sit amet, consectetur 
adipiscing elit;
Vivamus sit amet viverra arcu, sed ultricies
nulla.
Praesent egestas nunc mattis imperdiet
posuere.
Donec at nunc ac mauris8 suscipit venenatis.
Etiam quis neque tincidunt, porta odio vitae, 
scelerisque ante?
Nam 7elementum id enim eu congue;
Praesent egestas nunc mattis imperdiet posuere.
Sed finibus magna et mauris elementum tempus?
Class aptenT taciti sociosqu ad litora torquent
per cOnubia nostra, per inceptos himenaeos.
Phasellus iaculis sem ac congue egestas.
Donec accumsan convallis ipsum vitae lacinia.
Phasellus nunc augue, dApibus quis molestie 
iD, gravida a velit?
Aenean magna massa, scelerisque quis sagittis
at, pharetra a lectus?
Nam 7elementum id enim eu congue;
Phasellus nunc augue, dApibus quis molestie 
iD, gravida a velit?
Aenean magna massa, scelerisque quis sagittis
at, pharetra a lectus?
Ut auctor augue vel tincidunt tincidunt 555.
Donec at nunc ac mauris8 suscipit venenatis.
Phasellus nunc augue, dApibus quis molestie
iD, gravida a velit?
Suspendisse quis mi neque7.
Lorem ipsum dolor sit amet, consectetur 
adipiscing elit.
Aenean magna massa, scelerisque quis 
sagittis at, pharetra a lectus?
Donec at nunc ac mauris8 suscipit 
venenatis.
Maecenas 555 posuere velit efficitur, 
egestas nunc quis, dictum purus?
Etiam quis neque tincidunt, porta odio
vitae, scelerisque ante?
Fusce finibus sapien magna, quis 
scelerisque ex sodales tristique.
Fusce finibus sapien magna, quis 
scelerisque ex sodales tristique.
Suspendisse quis mi neque7.
Aenean sem ligula, laoreet ac sodales 
a, congue euismod neque;
Fusce finibus sapien magna, quis 
scelerisque ex sodales tristique.
Class aptenT taciti sociosqu ad litora 
torquent per cOnubia nostra, per 
inceptos himenaeos.
Maecenas 555 posuere velit efficitur, 
egestas nunc quis, dictum purus?
Phasellus nunc augue, dApibus quis 
molestie iD, gravida a velit?
Nam 7elementum id enim eu congue;
Etiam quis neque tincidunt, porta 
odio vitae, scelerisque ante?
Nam 7elementum id enim eu congue;
Nam 7elementum id enim eu congue;
Nam 7elementum id enim eu congue;
Maecenas 555 posuere velit efficitur,
egestas nunc quis, dictum purus?
Phasellus nunc augue, dApibus quis 
molestie iD, gravida a velit?
Fusce finibus sapien magna, quis 
scelerisque ex sodales tristique.
Praesent egestas nunc mattis 
imperdiet posuere.
Cras eget felis nibh?
Nulla facilisi.
Etiam quis neque tincidunt, porta 
odio vitae, scelerisque ante?
Sed finibus magna et mauris 
elementum tempus?
Nam 7elementum id enim eu congue;
Ut auctor augue vel tincidunt 
tincidunt 555.
Praesent egestas nunc mattis imperdiet posuere.
Donec accumsan convallis ipsum vitae lacinia.
Aenean sem ligula, laoreet ac sodales
a, congue euismod neque;
Donec accumsan convallis ipsum vitae
lacinia.
Donec at nunc ac mauris8 suscipit 
venenatis.
Nulla facilisi.
Donec accumsan convallis ipsum vitae 
lacinia.
Fusce finibus sapien magna, quis 
scelerisque ex sodales tristique.
Lorem ipsum dolor sit amet, consectetur 
adipiscing elit.
Ut auctor augue vel tincidunt tincidunt 555.
Phasellus nunc augue, dApibus quis 
molestie iD, gravida a velit?
Lorem ipsum dolor sit amet, consectetur 
adipiscing elit.
Lorem ipsum dolor sit amet, consectetur 
adipiscing elit.
Phasellus nunc augue, dApibus quis
molestie iD, gravida a velit?
Suspendisse quis mi neque7.
Suspendisse quis mi neque7.
Lorem ipsum dolor sit amet, consectetur 
adipiscing elit.
Donec accumsan convallis ipsum vitae 
lacinia.
Etiam quis neque tincidunt, porta odio
vitae, scelerisque ante?
Suspendisse quis mi neque7.
Maecenas 555 posuere velit efficitur, 
egestas nunc quis, dictum purus?
Maecenas 555 posuere velit efficitur, 
egestas nunc quis, dictum purus?
Ut auctor augue vel tincidunt tincidunt 
555.
Donec at nunc ac mauris8 suscipit 
venenatis.
Aenean magna massa, scelerisque quis 
sagittis at, pharetra a lectus?
Cras eget felis nibh?
Suspendisse quis mi neque7.
Donec accumsan convallis ipsum vitae 
lacinia.
Nulla facilisi.
Lorem ipsum dolor sit amet, consectetur
adipiscing elit.
Maecenas 555 posuere velit efficitur,
egestas nunc quis, dictum purus?
Phasellus nunc augue, dApibus quis 
molestie iD, gravida a velit?
Praesent egestas nunc mattis imperdiet 
posuere.
Nam 7elementum id enim eu congue;
Donec accumsan convallis ipsum vitae 
lacinia.
Etiam quis neque tincidunt, porta odio 
vitae, scelerisque ante?
Phasellus iaculis sem ac congue egestas.
Aenean magna massa, scelerisque quis 
sagittis at, pharetra a lectus?
Lorem ipsum dolor sit amet, consectetur 
adipiscing elit;
Suspendisse quis mi neque7.
Maecenas 555 posuere velit efficitur, 
egestas nunc quis, dictum purus?
Phasellus nunc augue, dApibus quis 
molestie iD, gravida a velit?
Aenean magna massa, scelerisque quis 
sagittis at, pharetra a lectus?
Sed finibus magna et mauris elementum 
tempus?
Donec accumsan convallis ipsum vitae 
lacinia.
Maecenas 555 posuere velit efficitur, 
egestas nunc quis, dictum purus?
Sed finibus magna et mauris elementum 
tempus?
Nulla facilisi.
Donec at nunc ac mauris8 suscipit 
venenatis.
Dragon flew away!
Количество предложений до 158 и
количество предложений после 107