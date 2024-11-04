window.addEventListener('keydown', function(event) {
    if ((event.ctrlKey || event.metaKey) && event.code === 'KeyS') {
        event.preventDefault();
        event.stopPropagation();
        return false;
    }

    if ((event.ctrlKey || event.metaKey) && event.shiftKey && event.code === 'KeyI') {
        event.preventDefault();
        event.stopPropagation();
        return false;
    }

    if ((event.ctrlKey || event.metaKey) && event.shiftKey && event.code === 'KeyC') {
        event.preventDefault();
        event.stopPropagation();
        return false;
    }

    if ((event.ctrlKey || event.metaKey) && event.shiftKey && event.code === 'KeyJ') {
        event.preventDefault();
        event.stopPropagation();
        return false;
    }

    if (event.code === 'F12') {
        event.preventDefault();
        event.stopPropagation();
        return false;
    }

    if ((event.ctrlKey || event.metaKey) && event.code === 'KeyU') {
        event.preventDefault();
        event.stopPropagation();
        return false;
    }
}, true);   

document.addEventListener('contextmenu', function(event) {
    event.preventDefault();
    const menu = document.getElementById('custom-context-menu');
    menu.style.top = `${event.pageY}px`;
    menu.style.left = `${event.pageX}px`;
    menu.classList.remove('hidden');
});

document.addEventListener('click', function() {
    const menu = document.getElementById('custom-context-menu');
    menu.classList.add('hidden');
});
