<script>
    import "./Components/Nav.svelte"
    import Nav from "./Components/Nav.svelte";
    import background from "./assets/background.png";
    const urlSearchParams = new URLSearchParams(window.location.search);
    const params = Object.fromEntries(urlSearchParams.entries());
    let id = params.id;
</script>

<main>
    <Nav />
    <img src={background} alt="" srcset="" width="100%">
    <div class="desc">

    </div>
    <img src="/video_feed{id ? `?id=${id}` : ""}" width="100%" alt="Whiteboard Video Stream"/>
</main>

<style>
    
</style>
