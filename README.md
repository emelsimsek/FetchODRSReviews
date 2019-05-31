# FetchODRSReviews
https://github.com/emelsimsek/FetchODRSReviews.git

Shows the json format for the json request and response format for the ODRS review server.

## Build

```bash
> sudo apt-get install  libsoup2.4-dev
> sudo apt-get install libjson-glib-dev

> gcc -g -Wall fetch_odrs_reviews.c -o fetch `pkg-config --cflags --libs libsoup-2.4 json-glib-1.0`
```

## Run
```bash
> ./fetch
Returned [
    {
        "app_id": "org.gnome.gedit.desktop",
        "date_created": 1461772858,
        "date_deleted": null,
        "description": "GEdit integrates really well into the GNOME desktop, and it is perfect for small plain-text editing tasks.\r\nUnfortunately it falls short as a text-editor for programming languages, where it often fails to indent code correctly, sometimes tries to spellcheck code, which makes no sense etc.\r\nSo, for small plaintext-editing tasks, it's a great tool, for programming not so much, because many features which would make GEdit a great programmers text-editor are sacrificed for the simplicity of the \"I just want to write a plaintext file\" usecase - which IMHO happens rarely to the non-technical user.\r\nWithout plugins, GEdit isn't as great as it could be (but fortunately, there are plugins!).",
        "distro": "Debian GNU/Linux",
        "karma_down": 1,
        "karma_up": 28,
        "locale": "en_DE",
        "rating": 60,
        "reported": 0,
        "review_id": 50,
        "score": 80,
        "summary": "Nice UI, but unclear scope",
        "user_display": "Matthias Klumpp",
        "user_hash": "2e7728c9ec496ec63353c58b420300e0cd458ec3",
        "user_skey": "cabb05fcf9d90bc6be694adf5f5f60d4bf53be11",
        "version": "3.20.1"
    },
    {
.....

```

