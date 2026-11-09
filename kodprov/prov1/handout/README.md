# Instruktioner

## I Windows

Efter att ha loggat in, sök efter "ThinLinc-klient" i sökrutan i
nedre vänstra hörnet och öppna den. Skriv in servern
"thinlinc.student.it.uu.se" och ditt eget användarnamn och
lösenord. Under "Advanced", kryssa i "End existing session". Om du
får frågan om du litar på denna host, tryck på "Continue". Det nya
fönstret som kommer upp kör nu på IT-institutionens
Linux-maskiner. Du kan göra fönstret helskärm genom att klicka på
rutan i det övre högra hörnet av fönstret. När du är klar ska du
logga ut genom menyn högst upp.

## I Linux

Öppna en terminal och skriv direkt `mkdir kodprov260112fm`. Gå till
denna mapp med `cd kodprov260112fm`. Ladda hem kodprovet till datorn
med följande kommando:

    curl -L --remote-name http://eliasc.github.io/ioopm/arkad.zip

Nu har du en zipfil med den utdelade koden. Packa upp den med
`unzip arkad.zip` (och ange lösenordet som du får under labben) så
får du *en annan* zipfil som heter `kodprov.zip`. Packar du upp
den med `unzip kodprov.zip` så får du en mapp `handout` som
innehåller uppgiften som ska lösas, samt en `Makefile` som du
använder för att lämna in kodprovet. Kom ihåg att spara!

## Att lämna in

Gå till mappen `kodprov260112fm`. Om du är osäker på var du är i
filsystemet kan du skriva `cd; cd kodprov260112fm`. Skriv `make
handin` för att lämna in kodprovet. Detta kommer att skapa en
zipfil som innehåller filen du ska lämna in (och inga andra
filer), och denna fil kommer att sparas på en plats där vi kan
rätta den. **OBS! Logga ut genom menyn när du är klar! Stäng inte
bara ThinLinc-fönstret.**

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

# Digiala lås

På Storsjöbadet i Östersund använder de digitala lås till alla
skåp i omklädningsrummen. Med ett armband man får i receptionen
kan man låsa vilket ledigt skåp som helst, och bara ens eget
armband kan låsa upp det igen. Det här kodprovet går ut på att
implementera ett liknande system, men där varje nyckel (armband)
kan låsa flera skåp.

**OBS! Det här kodprovet har många funktioner som ska
implementeras. De flesta funktioner är dock mycket enkla, så ta
det lugnt och läs noggrant.**

## Ett bibliotek för digitala lås och nycklar

I filen `lock.h` definieras ett gränssnitt för ett bibliotek som
hanterar lås och digitala nycklar. Typerna `lock_t` och
`digikey_t` representerar lås och nycklar (namnet `key_t` är redan
upptaget i C:s standardbibliotek). När en nyckel kopplas till ett
lås för första gången skapas en ny *session*, som representeras av
typen `session_t`. Nycklar känner till sin egen session, och låsta
lås känner till sessionen hos nyckeln som har låst dem. På det
sättet kan man försäkra sig om att endast en nyckel kan användas
för att låsa upp ett lås.

För att kunna veta när en session inte längre används av något lås
eller nyckel används referensräkning. En nyckel eller ett lås kan
*ansluta* till en session, vilket ökar referensräknaren med ett,
eller *avsluta* en session, vilket minskar referensräknaren med
ett. Typen `session_t` stödjer följande funktioner:

- `session_t *session_create()` -- skapa en ny session med
  referensräknaren satt till 1.

- `session_t *session_join(session_t *s)` -- öka `s`
  referensräknare med 1. Returnerar sessionen `s`.

- `session_t *session_drop(session_t *s)` -- minska `s`
  referensräknare med 1. Blir dess värde 0 ska sessionens minne
  frigöras och `NULL` returneras, annars returneras sessionen `s`.

Ett lås har ett ID-nummer (tänk numret på skåpet det hör till),
och en session. När låset är olåst är sessionen `NULL`, och annars
är det samma session som den nyckel som senast låste den. Typen
`lock_t` stödjer följande funktioner:

- `lock_t *lock_create(size_t id)` -- skapa ett nytt lås med ID
  `id` utan ansluten session.

- `void lock_destroy(lock_t *l)` -- förstör låset `l` och avsluta
  dess session (om den har någon).

- `size_t lock_get_id(lock_t *l)` -- returnera `l`:s ID.

- `session_t *lock_get_session(lock_t *l)` -- returnera `l`:s
  nuvarande session (`NULL` om låset är olåst).

- `bool lock_is_open(lock_t *l)` -- returnera `true` om `l` är
  olåst, annars `false`.

En nyckel bär på sin session (`NULL` om låset aldrig har använts).
När en nyckel väl har fått en session så är den oförändrad resten
av nyckelns livstid. Typen `digikey_t` stödjer följande
funktioner:

- `digikey_t *key_create()` -- skapa en ny nyckel utan ansluten session.

- `void key_destroy(digikey_t *k)` -- förstör nyckeln `k` och
  avsluta dess session (om den har någon).

- `session_t *key_get_session(digikey_t *k)` -- returnera `k`:s
  session (`NULL` om nyckeln aldrig har använts).

Slutligen finns en funktion som kopplar ihop ett lås och en nyckel:

- `bool connect_to_lock(lock_t *l, digikey_t *k)` -- anslut
  nyckeln `k` till låset `l`. Om låset `l` är olåst så låses det
  genom att anslutas till samma session som `k` (om `k` inte har
  någon session skapas en här). Om låset `l` redan är låst med
  samma session som `k` så låses det upp genom att avsluta sin
  session. I båda dessa fall ska `true` returneras. Om låset `l`
  redan är låst med en annan session än `k` så händer ingenting
  och `false` returneras.

**Din uppgift är att implementera nödvändiga funktioner och
strukturer från `lock.h` i filen `lock.c`. När du är klar borde du
kunna köra testerna genom Valgrind utan minnesfel eller -läckor.**

Följande exempel visar en interaktion med biblioteket:

```c
// Skapa en ny nyckel. Den har ingen session
digikey_t *k1 = key_create();
assert(key_get_session(k1) == NULL);

// Skapa ett nytt lås med ID 42. Det har ingen session och är öppen
lock_t *l1 = lock_create(42);
assert(lock_get_session(l1) == NULL);
assert(lock_is_open(l1));

// Lås l1 med nyckeln k1. De delar nu samma session (med refcount 2)
assert(connect_to_lock(l1, k1));
assert(!lock_is_open(l1));
assert(lock_get_session(l1) == key_get_session(k1));

// Samma nyckel kan användas för att låsa flera lås samtidigt
// (den delade sessionen får refcount 3)
lock_t *l2 = lock_create(84);
assert(connect_to_lock(l2, k1));
assert(!lock_is_open(l2));
assert(lock_get_session(l1) == lock_get_session(l2));

// En annan nyckel kan inte användas för att låsa upp ett lås
digikey_t *k2 = key_create();
assert(!connect_to_lock(l1, k2));
assert(key_get_session(k2) == NULL);

// När ett lås låses upp avslutas dess session
// (refcount blir 2)
assert(connect_to_lock(l2, k1));
assert(lock_is_open(l2));

// När en nyckel tas bort avslutas dess session
// (refcount blir 1)
key_destroy(k1);

// Låset l1 är fortfarande låst med samma session,
// men när det tas bort tas även dess session bort
// (eftersom sessionens refcount blir 0)
lock_destroy(l1);

// Återlämna resterande minne
lock_destroy(l2);
key_destroy(k2);
```

## Filer

- `lock.h` -- Gränssnittet som du ska implementera
- `lock.c` -- Du ska skriva din implementation här
- `lock_test.c` -- Tester för din implementation
- `Makefile`  -- En makefil med följande targets
  - `all`     -- Bygg programmet
  - `run`     -- Bygg och kör `main`-funktionen i `lock.c`
  - `test`    -- Bygg och kör testerna
  - `memrun`  -- Bygg och kör `main`-funktionen i `lock.c` med Valgrind
  - `memtest` -- Bygg och kör testerna med Valgrind
  - `clean`   -- Ta bort filer som har byggts

Notera att du kan skriva egna tester i den existerande
`main`-funktionen i `lock.c` och köra den med `make run`. Koden i
`main`-funktionen kommer inte att bedömas.

## Tips

- Du kan inte skriva C-kod som kontrollerar om minne har
  återlämnats eller inte. Om du inte känner dig säker på att
  använda Valgrind kanske det kan hjälpa att skriva ut en sessions
  refcount löpande.