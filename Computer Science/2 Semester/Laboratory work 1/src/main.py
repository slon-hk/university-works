class Edition:
    def __init__(self, name: str, price: int, age_limit: int, style: str):
        if not isinstance(name, str) or name == "":
            raise ValueError('Invalid value')
        if not isinstance(price, int) or price <= 0:
            raise ValueError('Invalid value')
        if not isinstance(age_limit, int) or age_limit <= 0:
            raise ValueError('Invalid value')
        if style not in ('c', 'b'):
            raise ValueError('Invalid value')

        self.name = name
        self.price = price
        self.age_limit = age_limit
        self.style = style


class Book(Edition):
    def __init__(self, name: str, price: int, age_limit: int, style: str,
                 author: str, hardcover: bool, pages: int):
        super().__init__(name, price, age_limit, style)

        if not isinstance(author, str) or author == "":
            raise ValueError('Invalid value')
        if not isinstance(hardcover, bool):
            raise ValueError('Invalid value')
        if not isinstance(pages, int) or pages <= 0:
            raise ValueError('Invalid value')

        self.author = author
        self.hardcover = hardcover
        self.pages = pages

    def __str__(self):
        return (f"Book: название {self.name}, цена {self.price}, "
                f"возрастное ограничение {self.age_limit}, стиль {self.style}, "
                f"автор {self.author}, твердый переплет {self.hardcover}, "
                f"количество страниц {self.pages}.")

    def __eq__(self, other):
        if isinstance(other, Book):
            return self.name == other.name and self.author == other.author
        return False


class Newspaper(Edition):
    def __init__(self, name: str, price: int, age_limit: int, style: str,
                 online_edition: bool, country: str, frequency: int):
        super().__init__(name, price, age_limit, style)

        if not isinstance(online_edition, bool):
            raise ValueError('Invalid value')
        if not isinstance(country, str) or country == "":
            raise ValueError('Invalid value')
        if not isinstance(frequency, int) or frequency <= 0:
            raise ValueError('Invalid value')

        self.online_edition = online_edition
        self.country = country
        self.frequency = frequency

    def __str__(self):
        return (f"Newspaper: название {self.name}, цена {self.price}, "
                f"возрастное ограничение {self.age_limit}, стиль {self.style}, "
                f"интернет издание {self.online_edition}, страна {self.country}, "
                f"периодичность {self.frequency}.")

    def __eq__(self, other):
        if isinstance(other, Newspaper):
            return self.name == other.name and self.country == other.country
        return False


class BookList(list):
    def __init__(self, name):
        super().__init__()
        self.name = name

    def append(self, p_object):
        if isinstance(p_object, Book):
            super().append(p_object)
        else:
            raise TypeError(f"Invalid type {type(p_object)}")

    def total_pages(self):
        return sum(book.pages for book in self)

    def print_count(self):
        print(len(self))


class NewspaperList(list):
    def __init__(self, name):
        super().__init__()
        self.name = name

    def extend(self, iterable):
        if not hasattr(iterable, '__iter__'):
            raise TypeError
        for item in iterable:
            if isinstance(item, Newspaper):
                super().append(item)

    def print_age(self):
        if self:
            print(min(n.age_limit for n in self))

    def print_total_price(self):
        print(sum(n.price for n in self))