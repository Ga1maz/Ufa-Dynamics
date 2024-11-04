function setupTabs(tabPrefix) {
    const tabsPrice = document.getElementById('tabsPrice');
    const listItem = tabsPrice ? tabsPrice.querySelectorAll('.tab-button') : [];
    const fivemTab = document.getElementById('fivemTab');
    const minecraftTab = document.getElementById('minecraftTab');
    const enterpriseTab = document.getElementById('enterpriseTab');
    const fivemTabButton = document.getElementById('fivemTabButton');
    const minecraftTabButton = document.getElementById('minecraftTabButton');
    const enterpriseTabButton = document.getElementById('enterpriseTabButton');

    if (fivemTabButton && minecraftTabButton && enterpriseTabButton) {
        fivemTabButton.addEventListener('click', () => {
            fivemTabButton.classList.add('active-tab');
            minecraftTabButton.classList.remove('active-tab');
            enterpriseTabButton.classList.remove('active-tab');
            fivemTab.style.display = 'grid';
            minecraftTab.style.display = 'none';
            enterpriseTab.style.display = 'none';
            fivemTabButton.classList.add('bg-black');
            fivemTabButton.classList.remove('bg-white/5');
            minecraftTabButton.classList.remove('bg-black');
            minecraftTabButton.classList.add('bg-white/5');
            enterpriseTabButton.classList.remove('bg-black');
            enterpriseTabButton.classList.add('bg-white/5');
        });

        minecraftTabButton.addEventListener('click', () => {
            fivemTabButton.classList.remove('active-tab');
            minecraftTabButton.classList.add('active-tab');
            enterpriseTabButton.classList.remove('active-tab');
            fivemTab.style.display = 'none';
            minecraftTab.style.display = 'grid';
            enterpriseTab.style.display = 'none';
            fivemTabButton.classList.remove('bg-black');
            fivemTabButton.classList.add('bg-white/5');
            minecraftTabButton.classList.remove('bg-white/5');
            minecraftTabButton.classList.add('bg-black');
            enterpriseTabButton.classList.remove('bg-black');
            enterpriseTabButton.classList.add('bg-white/5');
        });

        enterpriseTabButton.addEventListener('click', () => {
            fivemTabButton.classList.remove('active-tab');
            minecraftTabButton.classList.remove('active-tab');
            enterpriseTabButton.classList.add('active-tab');
            fivemTab.style.display = 'none';
            minecraftTab.style.display = 'none';
            enterpriseTab.style.display = 'grid';
            fivemTabButton.classList.remove('bg-black');
            fivemTabButton.classList.add('bg-white/5');
            minecraftTabButton.classList.remove('bg-black');
            minecraftTabButton.classList.add('bg-white/5');
            enterpriseTabButton.classList.add('bg-black');
            enterpriseTabButton.classList.remove('bg-white/5');
        });
    }
}

document.addEventListener('DOMContentLoaded', () => {
    setupTabs('pricing');
    setupTabs('pricing2');

    const currentYearElem = document.getElementById('current-year');
    if (currentYearElem) {
        currentYearElem.textContent = new Date().getFullYear();
    }

    fetch('/oferta/lib/index_files/footer.html')
        .then(response => {
            if (!response.ok) {
                throw new Error('Ошибка загрузки футера: ' + response.statusText);
            }
            return response.text();
        })
        .then(data => {
            document.getElementById('footer-placeholder').innerHTML = data;
        })
        .catch(error => {
            console.error('Ошибка при загрузке футера:', error);
            document.getElementById('footer-placeholder').innerHTML = "<p>Не удалось загрузить футер</p>";
        });
});


