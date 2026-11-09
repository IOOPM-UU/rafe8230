# Instruktioner

## I Windows

Efter att ha loggat in, sök efter "ThinLinc-klient" i sökrutan i
nedre vänstra hörnet och öppna den. Skriv in servern
"thinlinc.student.it.uu.se" och ditt eget användarnamn och
lösenord. Om du får frågan om du litar på denna host, tryck på
"Continue". Det nya fönstret som kommer upp kör nu på
IT-institutionens Linux-maskiner. Du kan göra fönstret helskärm
genom att klicka på rutan i det övre högra hörnet av fönstret.
Undvik att trycka på "x" eftersom detta kommer att avsluta din
anslutning!

## I Linux

Öppna en terminal och skriv direkt `mkdir kodprov251030`. Gå till
denna mapp med `cd kodprov251030`. Ladda hem kodprovet till datorn
med följande kommando:

    curl -L --remote-name http://eliasc.github.io/ioopm/ceylon.zip

Nu har du en zipfil med den utdelade koden. Packa upp den med
`unzip ceylon.zip` (och ange lösenordet som du får under labben) så
får du *en annan* zipfil som heter `kodprov.zip`. Packar du upp
den med `unzip kodprov.zip` så får du en mapp `handout` som
innehåller uppgiften som ska lösas, samt en `Makefile` som du
använder för att lämna in kodprovet.

## Att lämna in

Gå till mappen `kodprov251030`. Om du är osäker på var du är i
filsystemet kan du skriva `cd; cd kodprov251030`. Skriv `make
handin` för att lämna in kodprovet. Detta kommer att skapa en
zipfil som innehåller filen du ska lämna in (och inga andra
filer), och denna fil kommer att sparas på en plats där vi kan
rätta den.

## Allmänna regler

- Samma regler gäller som vid en skriftlig salstenta: inga
  mobiltelefoner, inga textmeddelanden, inga samtal med någon
  förutom lärare på kursen, oavsett medium.
- Du måste kunna identifiera dig.
- Du får ta med ett handskrivet papper (max A4). Du får skriva på
  båda sidorna, men får inte ta med ett förstoringsglas.
- Du får ta med en bok i fysisk form, på en läsplatta eller på en
  laptop. Du får inte köra några andra program på denna maskin och
  får inte använda den för något annat än att läsa kurslitteratur.
- Du måste skriva all kod själv, förutom koden som har delats ut.

# Spår med växlar

Vi vill modellera hur ett tåg färdas längs en järnväg med möjliga
växlar. En växel gör att tåget kan svänga in på ett sidospår
istället för att fortsätta rakt fram. Målet är att kunna bygga upp
en bana med spår och växlar och sedan simulera hur ett tåg rör sig
längs den (ordet "simulera" är möjligen väl grandiost för vad vi
ska åstadkomma).

## Ett bibliotek för spår med växlar

I filen `train.h` definieras ett gränssnitt för ett bibliotek som
modellerar spår. Typen `track_t` representerar ett
segment av spår. Ett spårsegment innehåller tre saker: en pekare
till nästa spårsegment (`NULL` om det är det sista segmentet), en
pekare till ett spårsegment som går att växla in på (`NULL` om det
inte går att växla) och en indikator huruvida det finns ett tåg på
segmentet eller inte. För enkelhets skull utgår vi från att ett
tåg är exakt så långt att det får plats på ett spårsegment.

Det finns fem funktioner i gränssnittet:

- `track_t *track_create(track_t *next)` — skapar ett spårsegment
  utan växlar och tåg vars nästa spårsegment är `next`.

- `void track_destroy(track_t *track)` — frigör allt minne som går
  att nå från och med spårsegmentet `track`, inklusive eventuella
  sidospår. Du kan anta att ett spår aldrig växlar tillbaka till
  sig själv.

- `bool track_add_switch(track_t *track, track_t *to)` — gör om
  `track` till en växel som leder in på sidospåret `to`.
  Returnerar `false` (och gör ingenting) om `track` redan är en
  växel, annars `true`.

- `bool track_has_train(track_t *track)` - Kontrollerar om `track` har
  ett tåg på sig just nu.

- `track_t *track_move(track_t *track, size_t steps, bool take_switch)` —
  Lägger till ett tåg på `track` och flyttar det `steps` steg
  framåt. Om `track` redan har ett tåg på sig flyttas detta tåg
  istället. Om `take_switch` är `true` och tåget passerar ett
  eller flera spårsegment som har växlar så viker tåget av i dessa
  växlar, annars fortsätter det rakt fram. Om det redan står ett
  tåg på ett spårsegment som ska passeras stannar tåget på
  spårsegmentet innan. Funktionen returnerar det spårsegment som
  tåget slutligen hamnar på. Om tåget når slutet av rälsen
  returneras det sista segmentet.

Följande exempel visar en interaktion med biblioteket:

```c
// Bygg banan a -> b -> c -> d -> e
track_t *e = track_create(NULL);
track_t *d = track_create(e);
track_t *c = track_create(d);
track_t *b = track_create(c);
track_t *a = track_create(b);

// Lägg till spår och en växel för att åstadkomma följande bana:
//             f -> g
//            /
// a -> b -> c -> d -> e

track_t *g = track_create(NULL);
track_t *f = track_create(g);
track_add_switch(c, f);

// Flytta ett tåg från a till d utan att ta växlar
assert(track_move(a, 3, false) == d);

// Nu har d ett tåg, men inte a (till exempel)
assert(track_has_train(d));
assert(!track_has_train(a));

// Flytta ett tåg från a till g genom att ta en växel
assert(track_move(a, 4, true) == g);

// Försök flytta ett tåg från a till e, stanna vid c eftersom d är upptaget
assert(track_move(a, 4, false) == c);

// Frigör utrymme
track_destroy(a);
```

**Din uppgift är att implementera nödvändiga funktioner och
strukturer från `train.h` i filen `train.c`. När du är klar borde du
kunna köra testerna genom Valgrind utan minnesfel eller -läckor.**

## Filer

- `train.h` -- Gränssnittet som du ska implementera
- `train.c` -- Du ska skriva din implementation här
- `train_test.c` -- Tester för din implementation
- `Makefile`  -- En makefil med följande targets
  - `all`     -- Bygg programmet
  - `run`     -- Bygg och kör `main`-funktionen i `train.c`
  - `test`    -- Bygg och kör testerna
  - `memrun`  -- Bygg och kör `main`-funktionen i `train.c` med Valgrind
  - `memtest` -- Bygg och kör testerna med Valgrind
  - `clean`   -- Ta bort filer som har byggts

Notera att du kan skriva egna tester i den existerande
`main`-funktionen i `train.c` och köra den med `make run`. Koden i
`main`-funktionen kommer inte att bedömas.

## Tips

- Tågen behöver inte någon dynamisk data, utan det räcker med att
  indikera om ett spårsegment har ett tåg på sig eller inte.
- Till skillnad från en länkad lista så beror "nästa spårsegment"
  på dels ifall det finns en växel och dels på ifall tåget ska
  växla eller inte. Överväg att skriva en hjälpfunktion som
  returnerar nästa spårsegment givet relevant information.
- Det engelska ordet för växel är "switch", vilket är ett
  nyckelord i C. Du måste alltså välja ett annat namn än det.
- Typen `size_t` är bara en icke-negativ `int`.